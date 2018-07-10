#include <gmock/gmock.h>

#include <chrono>
#include <codecvt>
#include <thread>

#include "Pipe.hpp"

namespace {
std::vector<char> string_as_vector(const std::string& str) {
  return std::vector<char>(str.begin(), str.end());
}

TEST(Pipe, ShouldBeUbique) {
  auto pipe1 = Pipe::create_unique();
  auto pipe2 = Pipe::create_unique();
  ASSERT_NE(nullptr, pipe1.get());
  ASSERT_NE(nullptr, pipe2.get());
  EXPECT_FALSE(pipe1->get_name().empty());
  EXPECT_FALSE(pipe2->get_name().empty());
  EXPECT_NE(pipe1->get_name(), pipe2->get_name());
}

TEST(Pipe, WriteRead) {
  auto pipe1 = Pipe::create_unique();
  ASSERT_NE(nullptr, pipe1);
  auto pipe2 = Pipe::open(pipe1->get_name());
  ASSERT_NE(nullptr, pipe2);

  const std::vector<char> expected_buffer = string_as_vector("test buffer");
  EXPECT_TRUE(pipe2->write(expected_buffer));

  std::vector<char> buffer;
  EXPECT_TRUE(pipe1->read(buffer));

  EXPECT_THAT(buffer, ::testing::ElementsAreArray(expected_buffer));
}

TEST(Pipe, WaitToConnect) {
  auto pipe1 = Pipe::create_unique();
  ASSERT_NE(nullptr, pipe1);

  const std::wstring name = pipe1->get_name();

  const auto start = std::chrono::system_clock::now();

  std::thread thread([name]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto pipe2 = Pipe::open(name);
  });
  EXPECT_TRUE(pipe1->wait());

  const auto duration = std::chrono::system_clock::now() - start;

  EXPECT_GE(
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count(),
      std::chrono::milliseconds(100).count());
  thread.join();
}

TEST(Pipe, OpenNotExisting) {
  auto pipe = Pipe::open(L"\\\\.\\pipe\\npp_not_existing");
  ASSERT_EQ(nullptr, pipe);
}

TEST(Pipe, CreatePipeWithSameName) {
  auto pipe1 = Pipe::create(L"\\\\.\\pipe\\npp_existing_name");
  ASSERT_NE(nullptr, pipe1);
  auto pipe2 = Pipe::create(L"\\\\.\\pipe\\npp_existing_name");
  ASSERT_EQ(nullptr, pipe2);
}
}  // namespace
