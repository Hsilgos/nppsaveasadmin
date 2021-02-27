#include <gmock/gmock.h>

#include <atomic>
#include <fstream>
#include <future>
#include <string>
#include <thread>

#include "Common.hpp"
#include "ExecutionThread.hpp"
#include "ICommand.hpp"
#include "IWinApiFunctions.hpp"
#include "MockWinApiFunctions.hpp"
#include "TestUtilities.hpp"

namespace {
const char* PipeName = "test_pipe";
const char* TestFileNameA1 = "test_file2.txt";
const wchar_t* TestFileNameW1 = L"test_file2.txt";
const std::string TestBuffer = "test_text";

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgReferee;

class MockCommand : public ICommand {
 public:
  using ICommand::ICommand;
  MOCK_METHOD2(execute, bool(const std::vector<char>&, std::vector<char>&));
};

struct FuncExecutorFixture : public ::testing::Test {
  const FileAutoremover file_auto_remove = FileAutoremover(TestFileNameA1);
  DefaultWinApiFunctions default_winapi_functions;

  ExecutionThread execution_thread;

  void check_file_exists() { check_file_content(TestBuffer); }

  void check_file_content(const std::string& expected_file_content) {
    std::ifstream read_file(TestFileNameA1);
    ASSERT_TRUE(read_file);
    std::string file_content;
    read_file >> file_content;
    EXPECT_EQ(expected_file_content, file_content);
  }

  void write_test_file() {
    std::ofstream write_file(TestFileNameA1);
    ASSERT_TRUE(write_file);
    write_file << TestBuffer;
  }
};

TEST_F(FuncExecutorFixture, ExecuteExit) {
  ASSERT_EQ(0, execution_thread.execution_result);
  ASSERT_TRUE(execution_thread.thread.joinable());
  execution_thread.stop(true);
  ASSERT_FALSE(execution_thread.thread.joinable());
  EXPECT_EQ(NoError, execution_thread.execution_result);
}

struct WriteFileFixture : public FuncExecutorFixture {
  HANDLE file_handle = INVALID_HANDLE_VALUE;
  WriteFileFixture() {
    register_default_commands(execution_thread.command_manager,
                              default_winapi_functions);
    file_handle = execute_create_file_w(
        *execution_thread.pipe_sender, *execution_thread.pipe_receiver,
        TestFileNameW1, GENERIC_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL);
    EXPECT_EQ(ERROR_SUCCESS, GetLastError());
    EXPECT_NE(INVALID_HANDLE_VALUE, file_handle);
  }

  void close_file() {
    if (INVALID_HANDLE_VALUE != file_handle) {
      EXPECT_TRUE(execute_close_handle(*execution_thread.pipe_sender,
                                       *execution_thread.pipe_receiver,
                                       file_handle));
      EXPECT_EQ(NoError, execution_thread.execution_result);
      file_handle = INVALID_HANDLE_VALUE;
    }
  }

  ~WriteFileFixture() { close_file(); }
};

struct CorruptedDataFixture : public FuncExecutorFixture {
  HANDLE file_handle = INVALID_HANDLE_VALUE;
  void open_file() {
    file_handle = execute_create_file_w(
        *execution_thread.pipe_sender, *execution_thread.pipe_receiver,
        TestFileNameW1, GENERIC_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL);
    EXPECT_EQ(ERROR_SUCCESS, GetLastError());
    EXPECT_NE(INVALID_HANDLE_VALUE, file_handle);
  }
  void close_file() {
    if (INVALID_HANDLE_VALUE != file_handle) {
      EXPECT_TRUE(CloseHandle(file_handle));
      file_handle = INVALID_HANDLE_VALUE;
    }
  }
  CorruptedDataFixture() {
    register_default_commands(execution_thread.command_manager,
                              default_winapi_functions);
  }
  ~CorruptedDataFixture() { close_file(); }

  std::vector<char> incorrect_return_data;
  MockWinApiFunctions mock_winapi;
  std::shared_ptr<MockCommand> incorrect_return_data_mock_command =
      std::make_shared<MockCommand>(mock_winapi);
  template <class ResultType>
  void prepare_test_incorrect_return_data_size(int resize_diff, Commands cmd) {
    prepare_vector_to_store_data<ResultType>(incorrect_return_data);
    const int new_size =
        static_cast<int>(incorrect_return_data.size()) + resize_diff;
    incorrect_return_data.resize(new_size);
    execution_thread.command_manager.register_command(
        cmd, incorrect_return_data_mock_command);
    EXPECT_CALL(*incorrect_return_data_mock_command, execute(_, _))
        .WillRepeatedly(
            DoAll(SetArgReferee<1>(incorrect_return_data), Return(true)));
  }
};

class WriteFileParametrizedTest : public WriteFileFixture,
                                  public ::testing::WithParamInterface<int> {};

TEST_P(WriteFileParametrizedTest, CreateWriteCloseFileAndGetType) {
  const int file_size = GetParam();
  std::string buffer_to_write(file_size, 'X');
  const std::string alphabet = "0123456789";
  std::generate(buffer_to_write.begin(), buffer_to_write.end(), [&alphabet]() {
    static size_t i = 0;
    if (i >= alphabet.size())
      i = 0;
    return alphabet[i++];
  });
  DWORD number_of_bytes_written = 0;
  EXPECT_TRUE(execute_write_file(
      *execution_thread.pipe_sender, *execution_thread.pipe_receiver,
      file_handle, buffer_to_write.c_str(), get_size_as_ulong(buffer_to_write),
      &number_of_bytes_written));
  EXPECT_EQ(ERROR_SUCCESS, GetLastError());
  EXPECT_EQ(FILE_TYPE_DISK, execute_get_file_type(
                                *execution_thread.pipe_sender,
                                *execution_thread.pipe_receiver, file_handle));
  EXPECT_EQ(ERROR_SUCCESS, GetLastError());
  EXPECT_EQ(buffer_to_write.size(), number_of_bytes_written);
  close_file();
  check_file_content(buffer_to_write);
}
INSTANTIATE_TEST_SUITE_P(WriteFileTestSequence,
                        WriteFileParametrizedTest,
                        ::testing::Values(20,
                                          WriteFileData::MaxBufferSize,
                                          WriteFileData::MaxBufferSize - 1,
                                          WriteFileData::MaxBufferSize + 1,
                                          WriteFileData::MaxBufferSize * 3,
                                          WriteFileData::MaxBufferSize * 3 - 1,
                                          WriteFileData::MaxBufferSize * 3 +
                                              1));

TEST_F(WriteFileFixture, ValidData_NoPtrToWritten) {
  EXPECT_TRUE(execute_write_file(*execution_thread.pipe_sender,
                                 *execution_thread.pipe_receiver, file_handle,
                                 TestBuffer.c_str(), get_size_as_ulong(TestBuffer), NULL));
  EXPECT_EQ(ERROR_SUCCESS, GetLastError());
  EXPECT_EQ(FILE_TYPE_DISK, execute_get_file_type(
                                *execution_thread.pipe_sender,
                                *execution_thread.pipe_receiver, file_handle));
  EXPECT_EQ(ERROR_SUCCESS, GetLastError());
  close_file();
  check_file_content(TestBuffer);
}

TEST_F(WriteFileFixture, ZeroBufferLenth) {
  DWORD number_of_bytes_written = 0;
  EXPECT_TRUE(execute_write_file(
      *execution_thread.pipe_sender, *execution_thread.pipe_receiver,
      file_handle, TestBuffer.c_str(), 0, &number_of_bytes_written));
  EXPECT_EQ(ERROR_SUCCESS, GetLastError());
  EXPECT_EQ(0, number_of_bytes_written);
}

TEST_F(WriteFileFixture, ZeroBufferLenth_NoPtrToWritten) {
  SetLastError(ERROR_SUCCESS);
  EXPECT_TRUE(execute_write_file(*execution_thread.pipe_sender,
                                 *execution_thread.pipe_receiver, file_handle,
                                 TestBuffer.c_str(), 0, NULL));
  EXPECT_EQ(ERROR_SUCCESS, GetLastError());
}

TEST_F(WriteFileFixture, NoBuffer) {
  DWORD number_of_bytes_written = 0;
  EXPECT_FALSE(execute_write_file(
      *execution_thread.pipe_sender, *execution_thread.pipe_receiver,
      file_handle, NULL, get_size_as_ulong(TestBuffer), &number_of_bytes_written));
  EXPECT_EQ(ERROR_INVALID_USER_BUFFER, GetLastError());
  EXPECT_EQ(0, number_of_bytes_written);
  SetLastError(ERROR_SUCCESS);
}

TEST_F(WriteFileFixture, NoBuffer_NoPtrToWritten) {
  EXPECT_FALSE(execute_write_file(*execution_thread.pipe_sender,
                                  *execution_thread.pipe_receiver, file_handle,
                                  NULL, get_size_as_ulong(TestBuffer), NULL));
  EXPECT_EQ(ERROR_INVALID_USER_BUFFER, GetLastError());
  SetLastError(ERROR_SUCCESS);
}

TEST_F(WriteFileFixture, NoBufferAndZeroBufferLenth) {
  DWORD number_of_bytes_written = 0;
  EXPECT_TRUE(execute_write_file(*execution_thread.pipe_sender,
                                 *execution_thread.pipe_receiver, file_handle,
                                 NULL, 0, &number_of_bytes_written));
  EXPECT_EQ(0, number_of_bytes_written);
  EXPECT_EQ(ERROR_SUCCESS, GetLastError());
}

TEST_F(WriteFileFixture, NoBufferAndZeroBufferLenth_NoPtrToWritten) {
  EXPECT_TRUE(execute_write_file(*execution_thread.pipe_sender,
                                 *execution_thread.pipe_receiver, file_handle,
                                 NULL, 0, NULL));
  EXPECT_EQ(ERROR_SUCCESS, GetLastError());
}

TEST_F(FuncExecutorFixture, CreateFileLastErrorIsAccesDenied) {
  MockWinApiFunctions mock_winapi;
  std::shared_ptr<MockCommand> mock_command =
      std::make_shared<MockCommand>(mock_winapi);
  std::vector<char> ret_data;
  CreateFileResult* create_file_result =
      prepare_vector_to_store_data<CreateFileResult>(ret_data);
  create_file_result->handle = INVALID_HANDLE_VALUE;
  create_file_result->last_error = ERROR_ACCESS_DENIED;
  execution_thread.command_manager.register_command(CreateFileWCmd,
                                                    mock_command);
  EXPECT_CALL(*mock_command, execute(_, _))
      .WillOnce(DoAll(SetArgReferee<1>(ret_data), Return(true)));
  const HANDLE handle = execute_create_file_w(
      *execution_thread.pipe_sender, *execution_thread.pipe_receiver,
      TestFileNameW1, GENERIC_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL);
  EXPECT_EQ(INVALID_HANDLE_VALUE, handle);
  EXPECT_EQ(ERROR_ACCESS_DENIED, GetLastError());
}

TEST_F(FuncExecutorFixture, CreateFileCorruptedData) {
  MockWinApiFunctions mock_winapi;
  std::shared_ptr<MockCommand> mock_command =
      std::make_shared<MockCommand>(mock_winapi);
  execution_thread.command_manager.register_command(CreateFileWCmd,
                                                    mock_command);
  EXPECT_CALL(*mock_command, execute(_, _)).WillOnce(Return(false));
  const HANDLE handle = execute_create_file_w(
      *execution_thread.pipe_sender, *execution_thread.pipe_receiver,
      TestFileNameW1, GENERIC_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL);
  EXPECT_EQ(INVALID_HANDLE_VALUE, handle);
  EXPECT_EQ(BadDataInBuffer, execution_thread.wait());
}

TEST_F(FuncExecutorFixture, CreateFileNoSuchCommandRegistered) {
  const HANDLE handle = execute_create_file_w(
      *execution_thread.pipe_sender, *execution_thread.pipe_receiver,
      TestFileNameW1, GENERIC_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL);
  EXPECT_EQ(INVALID_HANDLE_VALUE, handle);
  EXPECT_EQ(UnknownCommand, execution_thread.wait());
}

TEST_F(FuncExecutorFixture, CreateFileEmptyData) {
  MockWinApiFunctions mock_winapi;
  std::shared_ptr<MockCommand> mock_command =
      std::make_shared<MockCommand>(mock_winapi);
  execution_thread.command_manager.register_command(CreateFileWCmd,
                                                    mock_command);
  std::vector<char> ret_data;
  EXPECT_CALL(*mock_command, execute(_, _))
      .WillOnce(DoAll(SetArgReferee<1>(ret_data), Return(true)));
  const HANDLE handle = execute_create_file_w(
      *execution_thread.pipe_sender, *execution_thread.pipe_receiver,
      TestFileNameW1, GENERIC_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL);
  EXPECT_EQ(INVALID_HANDLE_VALUE, handle);
  EXPECT_EQ(ReturnBufferHasNoData, execution_thread.wait());
}

TEST_F(FuncExecutorFixture, CreateFileReturnedCorruptedDataLess) {
  std::vector<char> ret_data;
  prepare_vector_to_store_data<CreateFileResult>(ret_data);
  ret_data.resize(ret_data.size() - 1);
  MockWinApiFunctions mock_winapi;
  std::shared_ptr<MockCommand> mock_command =
      std::make_shared<MockCommand>(mock_winapi);
  execution_thread.command_manager.register_command(CreateFileWCmd,
                                                    mock_command);
  EXPECT_CALL(*mock_command, execute(_, _))
      .WillOnce(DoAll(SetArgReferee<1>(ret_data), Return(true)));
  const HANDLE handle = execute_create_file_w(
      *execution_thread.pipe_sender, *execution_thread.pipe_receiver,
      TestFileNameW1, GENERIC_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL);
  EXPECT_EQ(INVALID_HANDLE_VALUE, handle);
}

TEST_F(FuncExecutorFixture, CreateFileReturnedCorruptedDataMore) {
  std::vector<char> ret_data;
  prepare_vector_to_store_data<CreateFileResult>(ret_data);
  ret_data.resize(ret_data.size() + 1);
  MockWinApiFunctions mock_winapi;
  std::shared_ptr<MockCommand> mock_command =
      std::make_shared<MockCommand>(mock_winapi);
  execution_thread.command_manager.register_command(CreateFileWCmd,
                                                    mock_command);
  EXPECT_CALL(*mock_command, execute(_, _))
      .WillOnce(DoAll(SetArgReferee<1>(ret_data), Return(true)));
  const HANDLE handle = execute_create_file_w(
      *execution_thread.pipe_sender, *execution_thread.pipe_receiver,
      TestFileNameW1, GENERIC_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL);
  EXPECT_EQ(INVALID_HANDLE_VALUE, handle);
}

TEST_F(CorruptedDataFixture, WriteFileCorruptedData) {
  open_file();
  MockWinApiFunctions mock_winapi;
  std::shared_ptr<MockCommand> mock_command =
      std::make_shared<MockCommand>(mock_winapi);
  execution_thread.command_manager.register_command(WriteFileCmd, mock_command);
  EXPECT_CALL(*mock_command, execute(_, _)).WillOnce(Return(false));
  DWORD number_of_bytes_written = 0;
  EXPECT_FALSE(execute_write_file(*execution_thread.pipe_sender,
                                  *execution_thread.pipe_receiver, file_handle,
                                  TestBuffer.c_str(), get_size_as_ulong(TestBuffer),
                                  &number_of_bytes_written));
  EXPECT_EQ(BadDataInBuffer, execution_thread.wait());
}

TEST_F(CorruptedDataFixture, WriteFileCorruptedData_NoPtrToWritten) {
  open_file();
  MockWinApiFunctions mock_winapi;
  std::shared_ptr<MockCommand> mock_command =
      std::make_shared<MockCommand>(mock_winapi);
  execution_thread.command_manager.register_command(WriteFileCmd, mock_command);
  EXPECT_CALL(*mock_command, execute(_, _)).WillOnce(Return(false));
  EXPECT_FALSE(execute_write_file(*execution_thread.pipe_sender,
                                  *execution_thread.pipe_receiver, file_handle,
                                  TestBuffer.c_str(), get_size_as_ulong(TestBuffer), NULL));
  EXPECT_EQ(BadDataInBuffer, execution_thread.wait());
}

TEST_F(CorruptedDataFixture, WriteFileNoSuchCommandRegistered) {
  open_file();
  execution_thread.command_manager.erase_command(WriteFileCmd);
  EXPECT_FALSE(execute_write_file(*execution_thread.pipe_sender,
                                  *execution_thread.pipe_receiver, file_handle,
                                  TestBuffer.c_str(), get_size_as_ulong(TestBuffer), NULL));
  EXPECT_EQ(UnknownCommand, execution_thread.wait());
}

TEST_F(CorruptedDataFixture, WriteFileReturnedCorruptedDataLess) {
  open_file();
  prepare_test_incorrect_return_data_size<WriteFileResult>(-1, WriteFileCmd);
  DWORD number_of_bytes_written = 0;
  EXPECT_FALSE(execute_write_file(*execution_thread.pipe_sender,
                                  *execution_thread.pipe_receiver, file_handle,
                                  TestBuffer.c_str(), get_size_as_ulong(TestBuffer),
                                  &number_of_bytes_written));
  EXPECT_FALSE(execute_write_file(*execution_thread.pipe_sender,
                                  *execution_thread.pipe_receiver, file_handle,
                                  TestBuffer.c_str(), get_size_as_ulong(TestBuffer), NULL));
}

TEST_F(CorruptedDataFixture, WriteFileReturnedCorruptedDataMore) {
  open_file();
  prepare_test_incorrect_return_data_size<WriteFileResult>(1, WriteFileCmd);
  DWORD number_of_bytes_written = 0;
  EXPECT_FALSE(execute_write_file(*execution_thread.pipe_sender,
                                  *execution_thread.pipe_receiver, file_handle,
                                  TestBuffer.c_str(), get_size_as_ulong(TestBuffer),
                                  &number_of_bytes_written));
  EXPECT_FALSE(execute_write_file(*execution_thread.pipe_sender,
                                  *execution_thread.pipe_receiver, file_handle,
                                  TestBuffer.c_str(), get_size_as_ulong(TestBuffer), NULL));
}

TEST_F(CorruptedDataFixture, CloseHandleReturnedCorruptedDataLess) {
  open_file();
  prepare_test_incorrect_return_data_size<CloseHandleResult>(-1,
                                                             CloseHandleCmd);
  EXPECT_FALSE(execute_close_handle(*execution_thread.pipe_sender,
                                    *execution_thread.pipe_receiver,
                                    file_handle));
}

TEST_F(CorruptedDataFixture, CloseHandleReturnedCorruptedDataMore) {
  open_file();
  prepare_test_incorrect_return_data_size<CloseHandleResult>(1, CloseHandleCmd);
  EXPECT_FALSE(execute_close_handle(*execution_thread.pipe_sender,
                                    *execution_thread.pipe_receiver,
                                    file_handle));
}

TEST_F(CorruptedDataFixture, GetFileTypeReturnedCorruptedDataLess) {
  open_file();
  prepare_test_incorrect_return_data_size<GetFileTypeResult>(-1,
                                                             GetFileTypeCmd);
  EXPECT_FALSE(execute_get_file_type(*execution_thread.pipe_sender,
                                     *execution_thread.pipe_receiver,
                                     file_handle));
}

TEST_F(CorruptedDataFixture, GetFileTypeReturnedCorruptedDataMore) {
  open_file();
  prepare_test_incorrect_return_data_size<GetFileTypeResult>(1, GetFileTypeCmd);
  EXPECT_FALSE(execute_get_file_type(*execution_thread.pipe_sender,
                                     *execution_thread.pipe_receiver,
                                     file_handle));
}
}  // namespace
