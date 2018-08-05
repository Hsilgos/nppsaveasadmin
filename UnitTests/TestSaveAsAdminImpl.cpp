#include <gmock/gmock.h>

#include <fstream>

#include "ExecutionThread.hpp"
#include "IWinApiFunctions.hpp"
#include "MockWinApiFunctions.hpp"
#include "TestUtilities.hpp"
#include "plugin/Injection.hpp"
#include "plugin/SaveAsAdminImpl.hpp"

namespace {
using namespace ::testing;

const HANDLE ValidHandle1 = reinterpret_cast<HANDLE>(1);
const HANDLE ValidHandle2 = reinterpret_cast<HANDLE>(2);
const std::string TestFileNameA1 = "test_wfile1.txt";
const std::wstring TestFileNameW1 = L"test_wfile1.txt";
const std::string TestFileNameA1_Hooked = "test_wfile1.txt-hooked";
const std::wstring TestFileNameW1_Hooked = L"test_wfile1.txt-hooked";
// const std::string HookedAPrefix = "hooked-";
// const std::wstring HookedWPrefix = L"hooked-";

class MockAdminAccessRunner : public AdminAccessRunner {
 public:
  MOCK_METHOD3(run_admin_access,
               HANDLE(IWinApiFunctions&,
                      const std::wstring&,
                      const std::wstring&));
};

class TestWinApiFunctions {
 public:
  TestWinApiFunctions() {
    m_write_file = inject(
        WriteFile,
        make_injection_callback(*this, &TestWinApiFunctions::write_file));
    m_create_filea = inject(
        CreateFileA,
        make_injection_callback(*this, &TestWinApiFunctions::create_file_a));
    m_create_filew =
        inject(CreateFileW,
               make_injection_callback(
                   *this, &TestWinApiFunctions::create_file_w_denied_access));
    m_get_file_type = inject(
        GetFileType,
        make_injection_callback(*this, &TestWinApiFunctions::get_file_type));
    m_close_handle = inject(
        CloseHandle,
        make_injection_callback(*this, &TestWinApiFunctions::close_handle));
  }

  void allow_create() {
	  m_create_is_denied = false;
  }

  HANDLE create_file_w_denied_access(LPCWSTR file_name,
                                     DWORD desired_access,
                                     DWORD share_mode,
                                     LPSECURITY_ATTRIBUTES security_attributes,
                                     DWORD creation_disposition,
                                     DWORD flags_and_attributes,
                                     HANDLE template_file) {
	  if (m_create_is_denied) {
		  std::wstring file_name_str = file_name;
		  if (file_name_str.find(L"\\\\.\\pipe") == std::wstring::npos) {
			  SetLastError(ERROR_ACCESS_DENIED);
			  return INVALID_HANDLE_VALUE;
		  }
	  }
    return create_file_w(file_name, desired_access, share_mode,
                         security_attributes, creation_disposition,
                         flags_and_attributes, template_file);
  }

  BOOL write_file(HANDLE file_handle,
                  LPCVOID buffer,
                  DWORD number_of_bytes_to_write,
                  LPDWORD number_of_bytes_written,
                  LPOVERLAPPED overlapped) {
    return m_write_file->call_original(file_handle, buffer,
                                       number_of_bytes_to_write,
                                       number_of_bytes_written, overlapped);
  }

  HANDLE create_file_a(LPCSTR file_name,
                       DWORD desired_access,
                       DWORD share_mode,
                       LPSECURITY_ATTRIBUTES security_attributes,
                       DWORD creation_disposition,
                       DWORD flags_and_attributes,
                       HANDLE template_file) {
    return m_create_filea->call_original(
        file_name, desired_access, share_mode, security_attributes,
        creation_disposition, flags_and_attributes, template_file);
  }

  HANDLE create_file_w(LPCWSTR file_name,
                       DWORD desired_access,
                       DWORD share_mode,
                       LPSECURITY_ATTRIBUTES security_attributes,
                       DWORD creation_disposition,
                       DWORD flags_and_attributes,
                       HANDLE template_file) {
    return m_create_filew->call_original(
        file_name, desired_access, share_mode, security_attributes,
        creation_disposition, flags_and_attributes, template_file);
  }

  BOOL close_handle(HANDLE object) {
    return m_close_handle->call_original(object);
  }

  DWORD get_file_type(HANDLE file) {
    return m_get_file_type->call_original(file);
  }

 private:
  bool m_create_is_denied = true;
  injection_ptr_type(WriteFile) m_write_file;
  injection_ptr_type(CreateFileA) m_create_filea;
  injection_ptr_type(CreateFileW) m_create_filew;
  injection_ptr_type(GetFileType) m_get_file_type;
  injection_ptr_type(CloseHandle) m_close_handle;
};

struct SaveAsAdminImplFixture : public ::testing::Test {
  TestWinApiFunctions original_functions;
  ::testing::NiceMock<MockWinApiFunctions> mock_winapi_rename;
  ExecutionThread execution_thread;

  MockAdminAccessRunner mock_admin_access_runner;
  SaveAsAdminImpl save_as_admin_impl;

  SaveAsAdminImplFixture()
      : execution_thread(false), save_as_admin_impl(mock_admin_access_runner) {
    ON_CALL(mock_winapi_rename, create_file_a(_, _, _, _, _, _, _))
        .WillByDefault(
            Invoke(this, &SaveAsAdminImplFixture::create_file_a_rename));
    ON_CALL(mock_winapi_rename, create_file_w(_, _, _, _, _, _, _))
        .WillByDefault(
            Invoke(this, &SaveAsAdminImplFixture::create_file_w_rename));
    ON_CALL(mock_winapi_rename, write_file(_, _, _, _, _))
        .WillByDefault(
            Invoke(&original_functions, &TestWinApiFunctions::write_file));
    ON_CALL(mock_winapi_rename, close_handle(_))
        .WillByDefault(
            Invoke(&original_functions, &TestWinApiFunctions::close_handle));
    ON_CALL(mock_winapi_rename, get_file_type(_))
        .WillByDefault(
            Invoke(&original_functions, &TestWinApiFunctions::get_file_type));

    register_default_commands(execution_thread.command_manager,
                              mock_winapi_rename);
  }

  HANDLE create_file_w_rename(LPCWSTR file_name,
                              DWORD desired_access,
                              DWORD share_mode,
                              LPSECURITY_ATTRIBUTES security_attributes,
                              DWORD creation_disposition,
                              DWORD flags_and_attributes,
                              HANDLE template_file) {
    std::wstring new_name = file_name;
    new_name.append(L"-hooked");
    return original_functions.create_file_w(
        new_name.c_str(), desired_access, share_mode, security_attributes,
        creation_disposition, flags_and_attributes, template_file);
  }

  HANDLE create_file_a_rename(LPCSTR file_name,
                              DWORD desired_access,
                              DWORD share_mode,
                              LPSECURITY_ATTRIBUTES security_attributes,
                              DWORD creation_disposition,
                              DWORD flags_and_attributes,
                              HANDLE template_file) {
    std::string new_name = file_name;
    new_name.append("-hooked");
    return original_functions.create_file_a(
        new_name.c_str(), desired_access, share_mode, security_attributes,
        creation_disposition, flags_and_attributes, template_file);
  }

  ~SaveAsAdminImplFixture() { execution_thread.wait(); }

  HANDLE start_execute_thread1(IWinApiFunctions&,
                               const std::wstring& pipe_sender_name,
                               const std::wstring& pipe_receiver_name) {
    execution_thread.start(pipe_sender_name, pipe_receiver_name);
    return ValidHandle1;
  }

  void check_file_exists(const std::wstring& file_name,
                         const std::string& expected_content) {
    std::wifstream read_file(file_name);
    ASSERT_TRUE(read_file);
    std::wstring file_content;
    read_file >> file_content;
    std::wstring expected_wcontent(expected_content.begin(),
                                   expected_content.end());
    EXPECT_EQ(expected_wcontent, file_content);
  }

  void check_file_not_exists(const std::wstring& file_name) {
	  std::wifstream read_file(file_name);
	  ASSERT_FALSE(read_file);
  }

  void ProcessWriteFile(const std::wstring& file_name,
                        const std::string& data) {
    HANDLE handle = CreateFileW(file_name.c_str(), GENERIC_WRITE, 0, NULL,
                                CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    ASSERT_NE(INVALID_HANDLE_VALUE, handle);
    EXPECT_TRUE(WriteFile(handle, data.c_str(), data.size(), NULL, NULL));
    CloseHandle(handle);
  }
};

TEST_F(SaveAsAdminImplFixture, WriteFileIsHooked) {
  const auto remove_files =
      FileAutoremover({TestFileNameA1, TestFileNameA1_Hooked});
  EXPECT_CALL(mock_admin_access_runner, run_admin_access(_, _, _))
      .WillOnce(Invoke(this, &SaveAsAdminImplFixture::start_execute_thread1));

  const char* TestData = "Test_data";

  save_as_admin_impl.allow_process_file(TestFileNameW1);
  ProcessWriteFile(TestFileNameW1, TestData);
  save_as_admin_impl.cancel_process_file(TestFileNameW1);

  check_file_not_exists(TestFileNameW1);
  original_functions.allow_create();
  check_file_exists(TestFileNameW1_Hooked, TestData);
}

TEST_F(SaveAsAdminImplFixture, WriteFileIsHookedButNotProcessedBecauseNotAllowed) {
	const auto remove_files =
		FileAutoremover({ TestFileNameA1, TestFileNameA1_Hooked });
	EXPECT_CALL(mock_admin_access_runner, run_admin_access(_, _, _)).Times(0);

	HANDLE handle = CreateFileW(TestFileNameW1.c_str(), GENERIC_WRITE, 0, NULL,
		CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT_EQ(INVALID_HANDLE_VALUE, handle);

	check_file_not_exists(TestFileNameW1);
	check_file_not_exists(TestFileNameW1_Hooked);
}

TEST_F(SaveAsAdminImplFixture, WriteFileIsHookedButNotProcessedBecaueNotNeeded) {
	original_functions.allow_create();
	const auto remove_files =
		FileAutoremover({ TestFileNameA1, TestFileNameA1_Hooked });
	EXPECT_CALL(mock_admin_access_runner, run_admin_access(_, _, _)).Times(0);

	const char* TestData = "Test_data";

	save_as_admin_impl.allow_process_file(TestFileNameW1);
	ProcessWriteFile(TestFileNameW1, TestData);
	save_as_admin_impl.cancel_process_file(TestFileNameW1);

	check_file_exists(TestFileNameW1, TestData);
	check_file_not_exists(TestFileNameW1_Hooked);
}

}  // namespace
