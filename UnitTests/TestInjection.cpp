#include <gmock/gmock.h>

#include "plugin/AdminAccess.h"
#include "plugin/FileOperations.h"
#include "plugin/Injection.h"
#include "plugin/NppSaveAsAdminImpl.h"

#include "CommandManager.h"
#include "CommandProcessor.h"

#include "Pipe.h"

#include <thread>

#include <cstdio>
#include <fstream>

namespace {
const char* TestFileName1 = "test_file1.txt";
const std::string TestBuffer = "test_text";
const HANDLE ValidHandle = reinterpret_cast<HANDLE>(1);

const HANDLE ValidHandle1 = reinterpret_cast<HANDLE>(1);
const HANDLE ValidHandle2 = reinterpret_cast<HANDLE>(2);

using ::testing::_;
using ::testing::Return;
class MockFileOperations {
 public:
  MOCK_METHOD7(TestCreateFileA,
               HANDLE(LPCSTR,
                      DWORD,
                      DWORD,
                      LPSECURITY_ATTRIBUTES,
                      DWORD,
                      DWORD,
                      HANDLE));
  MOCK_METHOD5(TestWriteFile,
               BOOL(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED));
  MOCK_METHOD1(TestCloseHandle, BOOL(HANDLE));
};

class InjectionFixture : public ::testing::Test {
 public:
  InjectionFixture() { std::remove(TestFileName1); }
  ~InjectionFixture() { std::remove(TestFileName1); }
  void configure_mock_default() {
    ON_CALL(file_operations, TestCreateFileA(_, _, _, _, _, _, _))
        .WillByDefault(Return(ValidHandle));
    ON_CALL(file_operations, TestWriteFile(_, _, _, _, _))
        .WillByDefault(Return(TRUE));
    ON_CALL(file_operations, TestCloseHandle(_)).WillByDefault(Return(TRUE));
  }

  template <class Rest, class... Args>
  auto test_callback(Rest (MockFileOperations::*member)(Args...)) {
    return make_injection_callback(file_operations, member);
  }

  ::testing::StrictMock<MockFileOperations> file_operations;

  HANDLE create_file_default() {
    return CreateFileA(TestFileName1, GENERIC_WRITE, 0, NULL, CREATE_NEW,
                       FILE_ATTRIBUTE_NORMAL, NULL);
  }
  void write_file_with_win_api() {
    HANDLE file = create_file_default();
    ASSERT_NE(INVALID_HANDLE_VALUE, file);
    EXPECT_TRUE(
        WriteFile(file, TestBuffer.c_str(), TestBuffer.size(), NULL, NULL));
    CloseHandle(file);
  }

  void check_file_exists() {
    std::ifstream read_file(TestFileName1);
    ASSERT_TRUE(read_file);
    std::string file_content;
    read_file >> file_content;
    EXPECT_EQ(TestBuffer, file_content);
  }

  void check_file_not_exists() {
    std::ifstream read_file(TestFileName1);
    EXPECT_FALSE(read_file);
  }
};

TEST_F(InjectionFixture, WriteFileWithoutInjection) {
  write_file_with_win_api();
  check_file_exists();
}

TEST_F(InjectionFixture, CreateFileIsHookedAndReleased) {
  EXPECT_CALL(file_operations, TestCreateFileA(_, _, _, _, _, _, _))
      .WillOnce(Return(ValidHandle));
  auto create_file =
      inject_in_module("Kernel32.dll", CreateFileA,
                       test_callback(&MockFileOperations::TestCreateFileA));
  HANDLE file_handle = create_file_default();
  ASSERT_EQ(ValidHandle, file_handle);

  create_file.reset();
  file_handle = create_file_default();
  ASSERT_NE(INVALID_HANDLE_VALUE, file_handle);
  EXPECT_TRUE(WriteFile(file_handle, TestBuffer.c_str(), TestBuffer.size(),
                        NULL, NULL));
  CloseHandle(file_handle);

  check_file_exists();
}

TEST_F(InjectionFixture, CreateFileIsHookedAndReleased_NoModule) {
  EXPECT_CALL(file_operations, TestCreateFileA(_, _, _, _, _, _, _))
      .WillOnce(Return(ValidHandle));
  auto create_file =
      inject(CreateFileA, test_callback(&MockFileOperations::TestCreateFileA));
  HANDLE file_handle = create_file_default();
  ASSERT_EQ(ValidHandle, file_handle);

  create_file.reset();
  file_handle = create_file_default();
  ASSERT_NE(INVALID_HANDLE_VALUE, file_handle);
  EXPECT_TRUE(WriteFile(file_handle, TestBuffer.c_str(), TestBuffer.size(),
                        NULL, NULL));
  CloseHandle(file_handle);

  check_file_exists();
}

TEST_F(InjectionFixture, TryToInjectInWrongModule) {
  EXPECT_THROW(
      inject_in_module("WrongModule.dll", CreateFileA,
                       test_callback(&MockFileOperations::TestCreateFileA)),
      std::logic_error);
}

TEST_F(InjectionFixture, TryToInjectTwice_WithSameMacro) {
  auto inject_same_function = [this]() {
    return inject(CreateFileA,
                  test_callback(&MockFileOperations::TestCreateFileA));
  };
  auto injection = inject_same_function();
  EXPECT_THROW(inject_same_function(), std::logic_error);
}

TEST_F(InjectionFixture, InjectTwice_LastAliveReceivesOperations) {
  ::testing::StrictMock<MockFileOperations> file_operations1;
  ::testing::StrictMock<MockFileOperations> file_operations2;

  EXPECT_CALL(file_operations1, TestCreateFileA(_, _, _, _, _, _, _))
      .WillOnce(Return(ValidHandle1));
  EXPECT_CALL(file_operations2, TestCreateFileA(_, _, _, _, _, _, _))
      .WillOnce(Return(ValidHandle2));

  auto create_file1 = inject(
      CreateFileA, make_injection_callback(
                       file_operations1, &MockFileOperations::TestCreateFileA));
  auto create_file2 = inject(
      CreateFileA, make_injection_callback(
                       file_operations2, &MockFileOperations::TestCreateFileA));

  ASSERT_EQ(ValidHandle2, create_file_default());

  create_file2.reset();

  ASSERT_EQ(ValidHandle1, create_file_default());

  create_file1.reset();

  write_file_with_win_api();
  check_file_exists();
}

TEST_F(InjectionFixture, InjectTwice_CallOriginalCallsPrevious) {
  ::testing::StrictMock<MockFileOperations> file_operations1;
  ::testing::StrictMock<MockFileOperations> file_operations2;

  EXPECT_CALL(file_operations1, TestCreateFileA(_, _, _, _, _, _, _))
      .WillOnce(Return(ValidHandle1));

  auto create_file1 = inject(
      CreateFileA, make_injection_callback(
                       file_operations1, &MockFileOperations::TestCreateFileA));
  auto create_file2 = inject(
      CreateFileA, make_injection_callback(
                       file_operations2, &MockFileOperations::TestCreateFileA));

  ASSERT_EQ(ValidHandle1, create_file2->call_original(
                              TestFileName1, GENERIC_WRITE, 0, NULL, CREATE_NEW,
                              FILE_ATTRIBUTE_NORMAL, NULL));
}

TEST_F(InjectionFixture, InjectThreeTimes_ResetMiddle) {
  ::testing::StrictMock<MockFileOperations> file_operations1;
  ::testing::StrictMock<MockFileOperations> file_operations2;
  ::testing::StrictMock<MockFileOperations> file_operations3;

  EXPECT_CALL(file_operations1, TestCreateFileA(_, _, _, _, _, _, _))
      .WillOnce(Return(ValidHandle1));

  auto create_file1 = inject(
      CreateFileA, make_injection_callback(
                       file_operations1, &MockFileOperations::TestCreateFileA));
  auto create_file2 = inject(
      CreateFileA, make_injection_callback(
                       file_operations2, &MockFileOperations::TestCreateFileA));
  auto create_file3 = inject(
      CreateFileA, make_injection_callback(
                       file_operations3, &MockFileOperations::TestCreateFileA));

  create_file2.reset();

  ASSERT_EQ(ValidHandle1, create_file3->call_original(
                              TestFileName1, GENERIC_WRITE, 0, NULL, CREATE_NEW,
                              FILE_ATTRIBUTE_NORMAL, NULL));
}

TEST_F(InjectionFixture, CallOriginalFunctionAfterHook) {
  auto create_file =
      inject_in_module("Kernel32.dll", CreateFileA,
                       test_callback(&MockFileOperations::TestCreateFileA));
  HANDLE file_handle =
      create_file->call_original(TestFileName1, GENERIC_WRITE, 0, NULL,
                                 CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
  ASSERT_NE(INVALID_HANDLE_VALUE, file_handle);
  EXPECT_TRUE(WriteFile(file_handle, TestBuffer.c_str(), TestBuffer.size(),
                        NULL, NULL));
  CloseHandle(file_handle);

  check_file_exists();
}

TEST_F(InjectionFixture, WriteFileWithInjection) {
  EXPECT_CALL(file_operations, TestCreateFileA(_, _, _, _, _, _, _))
      .WillOnce(Return(ValidHandle));
  EXPECT_CALL(file_operations, TestWriteFile(_, _, _, _, _))
      .WillOnce(Return(TRUE));
  EXPECT_CALL(file_operations, TestCloseHandle(_)).WillOnce(Return(TRUE));

  auto create_file =
      inject_in_module("Kernel32.dll", CreateFileA,
                       test_callback(&MockFileOperations::TestCreateFileA));
  auto write_file =
      inject_in_module("Kernel32.dll", WriteFile,
                       test_callback(&MockFileOperations::TestWriteFile));
  auto close_handle =
      inject_in_module("Kernel32.dll", CloseHandle,
                       test_callback(&MockFileOperations::TestCloseHandle));

  write_file_with_win_api();
  check_file_not_exists();

  create_file.reset();
  write_file.reset();
  close_handle.reset();

  write_file_with_win_api();
  check_file_exists();
}
}  // namespace
