#include <gmock/gmock.h>

#include <Windows.h>

#include "UniqueHandle.hpp"

namespace {
using namespace testing;

const HANDLE ValidHandle = reinterpret_cast<HANDLE>(1);
const HANDLE ValidHandle2 = reinterpret_cast<HANDLE>(2);

class TestUniqueHandle : public Test {
 public:
  TestUniqueHandle() { SetLastError(0); }

  ~TestUniqueHandle() { EXPECT_EQ(0, GetLastError()); }
};

TEST_F(TestUniqueHandle, DefaultCtor) {
  UniqueHandle<> unique_handle;
  EXPECT_FALSE(unique_handle);
  EXPECT_EQ(INVALID_HANDLE_VALUE, unique_handle.get());
  EXPECT_EQ(INVALID_HANDLE_VALUE, *unique_handle);

  EXPECT_EQ(INVALID_HANDLE_VALUE, unique_handle);
}

TEST_F(TestUniqueHandle, CtorWithParamAndInvalidValue) {
  const UniqueHandle<> unique_handle;

  UniqueHandle<> unique_handle2(INVALID_HANDLE_VALUE);
  EXPECT_FALSE(unique_handle2);
  EXPECT_EQ(INVALID_HANDLE_VALUE, unique_handle2.get());
  EXPECT_EQ(INVALID_HANDLE_VALUE, *unique_handle2);

  EXPECT_TRUE(unique_handle == unique_handle2);
  EXPECT_FALSE(unique_handle != unique_handle2);
}

TEST_F(TestUniqueHandle, CustomDeleterAndValidHandle) {
  MockFunction<void(HANDLE)> deleter;
  EXPECT_CALL(deleter, Call(ValidHandle));
  UniqueHandle<std::function<void(HANDLE)>> unique_handle(
      ValidHandle, deleter.AsStdFunction());
  EXPECT_TRUE(unique_handle);
  EXPECT_NE(INVALID_HANDLE_VALUE, unique_handle.get());
  EXPECT_NE(INVALID_HANDLE_VALUE, *unique_handle);

  EXPECT_TRUE(INVALID_HANDLE_VALUE != unique_handle);
  EXPECT_TRUE(unique_handle != INVALID_HANDLE_VALUE);
  EXPECT_FALSE(INVALID_HANDLE_VALUE == unique_handle);
  EXPECT_FALSE(unique_handle == INVALID_HANDLE_VALUE);
}

TEST_F(TestUniqueHandle, CustomDeleterAndInvalidHandle) {
  MockFunction<void(HANDLE)> deleter;
  EXPECT_CALL(deleter, Call(_)).Times(0);
  UniqueHandle<std::function<void(HANDLE)>> unique_handle(
      INVALID_HANDLE_VALUE, deleter.AsStdFunction());
  EXPECT_FALSE(unique_handle);
  EXPECT_EQ(INVALID_HANDLE_VALUE, unique_handle.get());
  EXPECT_EQ(INVALID_HANDLE_VALUE, *unique_handle);

  EXPECT_FALSE(INVALID_HANDLE_VALUE != unique_handle);
  EXPECT_FALSE(unique_handle != INVALID_HANDLE_VALUE);
  EXPECT_TRUE(INVALID_HANDLE_VALUE == unique_handle);
  EXPECT_TRUE(unique_handle == INVALID_HANDLE_VALUE);
}

TEST_F(TestUniqueHandle, ReleaseWithValidHandle) {
  MockFunction<void(HANDLE)> deleter;
  EXPECT_CALL(deleter, Call(_)).Times(0);
  UniqueHandle<std::function<void(HANDLE)>> unique_handle(
      ValidHandle, deleter.AsStdFunction());
  EXPECT_EQ(ValidHandle, unique_handle.release());

  EXPECT_FALSE(unique_handle);
}

TEST_F(TestUniqueHandle, ReleaseWithInvalidHandle) {
  MockFunction<void(HANDLE)> deleter;
  EXPECT_CALL(deleter, Call(_)).Times(0);
  UniqueHandle<std::function<void(HANDLE)>> unique_handle(
      INVALID_HANDLE_VALUE, deleter.AsStdFunction());
  EXPECT_EQ(INVALID_HANDLE_VALUE, unique_handle.release());

  EXPECT_FALSE(unique_handle);
}

TEST_F(TestUniqueHandle, ResetWithValidHandle) {
  MockFunction<void(HANDLE)> deleter;
  EXPECT_CALL(deleter, Call(ValidHandle)).RetiresOnSaturation();
  UniqueHandle<std::function<void(HANDLE)>> unique_handle(
      ValidHandle, deleter.AsStdFunction());
  unique_handle.reset();
  EXPECT_CALL(deleter, Call(_)).Times(0);

  EXPECT_FALSE(unique_handle);
}

TEST_F(TestUniqueHandle, ResetWithInvalidHandle) {
  MockFunction<void(HANDLE)> deleter;
  EXPECT_CALL(deleter, Call(_)).Times(0);
  UniqueHandle<std::function<void(HANDLE)>> unique_handle(
      INVALID_HANDLE_VALUE, deleter.AsStdFunction());
  unique_handle.reset();

  EXPECT_FALSE(unique_handle);
}

TEST_F(TestUniqueHandle, MoveCtorFromValid) {
  MockFunction<void(HANDLE)> deleter;
  EXPECT_CALL(deleter, Call(_)).Times(0).RetiresOnSaturation();
  UniqueHandle<std::function<void(HANDLE)>> unique_handle1(
      ValidHandle, deleter.AsStdFunction());

  UniqueHandle<std::function<void(HANDLE)>> unique_handle2(
      std::move(unique_handle1));

  EXPECT_FALSE(unique_handle1);
  EXPECT_TRUE(unique_handle2);

  EXPECT_CALL(deleter, Call(ValidHandle));
}

TEST_F(TestUniqueHandle, MoveCtorFromInvalid) {
  MockFunction<void(HANDLE)> deleter;
  EXPECT_CALL(deleter, Call(_)).Times(0);
  UniqueHandle<std::function<void(HANDLE)>> unique_handle1(
      INVALID_HANDLE_VALUE, deleter.AsStdFunction());

  UniqueHandle<std::function<void(HANDLE)>> unique_handle2(
      std::move(unique_handle1));

  EXPECT_FALSE(unique_handle1);
  EXPECT_FALSE(unique_handle2);
}

TEST_F(TestUniqueHandle, MoveEqFromValidToInvalid) {
  MockFunction<void(HANDLE)> deleter;
  EXPECT_CALL(deleter, Call(_)).Times(0).RetiresOnSaturation();
  UniqueHandle<std::function<void(HANDLE)>> unique_handle1(
      ValidHandle, deleter.AsStdFunction());

  UniqueHandle<std::function<void(HANDLE)>> unique_handle2;

  unique_handle2 = std::move(unique_handle1);

  EXPECT_FALSE(unique_handle1);
  EXPECT_TRUE(unique_handle2);

  EXPECT_CALL(deleter, Call(ValidHandle));
}

TEST_F(TestUniqueHandle, MoveEqFromInvalidToInvalid) {
  MockFunction<void(HANDLE)> deleter;
  EXPECT_CALL(deleter, Call(_)).Times(0);
  UniqueHandle<std::function<void(HANDLE)>> unique_handle1(
      INVALID_HANDLE_VALUE, deleter.AsStdFunction());

  UniqueHandle<std::function<void(HANDLE)>> unique_handle2;

  unique_handle2 = std::move(unique_handle1);

  EXPECT_FALSE(unique_handle1);
  EXPECT_FALSE(unique_handle2);
}

TEST_F(TestUniqueHandle, MoveEqFromInvalidToValid) {
  MockFunction<void(HANDLE)> deleter1;
  EXPECT_CALL(deleter1, Call(_)).Times(0);
  UniqueHandle<std::function<void(HANDLE)>> unique_handle1(
      INVALID_HANDLE_VALUE, deleter1.AsStdFunction());

  MockFunction<void(HANDLE)> deleter2;
  EXPECT_CALL(deleter2, Call(ValidHandle)).RetiresOnSaturation();
  UniqueHandle<std::function<void(HANDLE)>> unique_handle2(
      ValidHandle, deleter2.AsStdFunction());

  unique_handle2 = std::move(unique_handle1);

  EXPECT_CALL(deleter2, Call(_)).Times(0);

  EXPECT_FALSE(unique_handle1);
  EXPECT_FALSE(unique_handle2);
}

TEST_F(TestUniqueHandle, OperatorEqHandleToInvalid) {
  MockFunction<void(HANDLE)> deleter;
  EXPECT_CALL(deleter, Call(_)).Times(0).RetiresOnSaturation();
  UniqueHandle<std::function<void(HANDLE)>> unique_handle(
      INVALID_HANDLE_VALUE, deleter.AsStdFunction());

  unique_handle = ValidHandle;

  EXPECT_CALL(deleter, Call(ValidHandle)).RetiresOnSaturation();

  unique_handle = ValidHandle2;

  EXPECT_CALL(deleter, Call(ValidHandle2));
}
}  // namespace
