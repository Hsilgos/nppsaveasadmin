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
using ::testing::Return;
using ::testing::_;
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
  InjectionFixture() {
    std::remove(TestFileName1);
    s_file_operations =
        std::make_unique<::testing::NiceMock<MockFileOperations>>();
  }
  ~InjectionFixture() {
    s_file_operations.reset();
    std::remove(TestFileName1);
  }
  void configure_mock_default() {
    ON_CALL(*s_file_operations, TestCreateFileA(_, _, _, _, _, _, _))
        .WillByDefault(Return(ValidHandle));
    ON_CALL(*s_file_operations, TestWriteFile(_, _, _, _, _))
        .WillByDefault(Return(TRUE));
    ON_CALL(*s_file_operations, TestCloseHandle(_)).WillByDefault(Return(TRUE));
  }

  static std::unique_ptr<::testing::NiceMock<MockFileOperations>>
      s_file_operations;

  static HANDLE WINAPI
  TestCreateFileA(LPCSTR lpFileName,
                  DWORD dwDesiredAccess,
                  DWORD dwShareMode,
                  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                  DWORD dwCreationDisposition,
                  DWORD dwFlagsAndAttributes,
                  HANDLE hTemplateFile) {
    return s_file_operations->TestCreateFileA(
        lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
        dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
  }
  static BOOL WINAPI TestWriteFile(HANDLE hFile,
                                   LPCVOID lpBuffer,
                                   DWORD nNumberOfBytesToWrite,
                                   LPDWORD lpNumberOfBytesWritten,
                                   LPOVERLAPPED lpOverlapped) {
    return s_file_operations->TestWriteFile(
        hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten,
        lpOverlapped);
  }
  static BOOL WINAPI TestCloseHandle(HANDLE hObject) {
    return s_file_operations->TestCloseHandle(hObject);
  }
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

std::unique_ptr<::testing::NiceMock<MockFileOperations>>
    InjectionFixture::s_file_operations;

TEST_F(InjectionFixture, WriteFileWithoutInjection) {
  write_file_with_win_api();
  check_file_exists();
}

TEST_F(InjectionFixture, CreteFileIsHookedAndReleased) {
  EXPECT_CALL(*s_file_operations, TestCreateFileA(_, _, _, _, _, _, _))
      .WillOnce(Return(ValidHandle));
  auto create_file = inject("Kernel32.dll", "CreateFileA", &TestCreateFileA);
  HANDLE file_handle = create_file_default();
  ASSERT_EQ(ValidHandle, file_handle);

  create_file.reset();
  file_handle = create_file_default();
  ASSERT_NE(INVALID_HANDLE_VALUE, file_handle);
  CloseHandle(file_handle);
}

TEST_F(InjectionFixture, CallOriginalFunctionAfterHook) {
  EXPECT_CALL(*s_file_operations, TestCreateFileA(_, _, _, _, _, _, _))
    .Times(0);
  auto create_file = inject("Kernel32.dll", "CreateFileA", &TestCreateFileA);
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
  configure_mock_default();

  auto create_file = inject("Kernel32.dll", "CreateFileA", &TestCreateFileA);
  auto write_file = inject("Kernel32.dll", "WriteFile", &TestWriteFile);
  auto close_handle = inject("Kernel32.dll", "CloseHandle", &TestCloseHandle);

  write_file_with_win_api();
  check_file_not_exists();

  create_file.reset();
  write_file.reset();
  close_handle.reset();

  write_file_with_win_api();
  check_file_exists();
}
}  // namespace
