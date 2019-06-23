#include "ICommand.hpp"
#include "IWinApiFunctions.hpp"

class WriteFileCommand : public ICommand {
 public:
  using ICommand::ICommand;

 private:
  bool execute(const std::vector<char>& data,
               std::vector<char>& ret_data) override {
    const WriteFileData* twfd =
        get_command_data_from_vector<WriteFileData>(data);
    if (!twfd)
      return false;
    WriteFileResult* result =
        prepare_vector_to_store_data<WriteFileResult>(ret_data);

    DWORD written = 0;

    const BOOL write_result = m_winapi.write_file(
        twfd->handle, twfd->buffer_is_null ? nullptr : twfd->buffer,
        twfd->num_bytes_to_write, &written, NULL);

    result->success = (TRUE == write_result);
    result->last_error = GetLastError();
    result->bytes_written = written;

    return true;
  }
};

std::unique_ptr<ICommand> make_write_file_command(
    IWinApiFunctions& original_functions) {
  return std::make_unique<WriteFileCommand>(original_functions);
}
