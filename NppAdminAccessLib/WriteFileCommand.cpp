#include "ICommand.h"

class WriteFileCommand : public ICommand {
  bool execute(const std::vector<char>& data,
               std::vector<char>& ret_data) override {
    const WriteFileData* twfd =
        get_command_data_from_vector<WriteFileData>(data);
    if (!twfd)
      return false;
    WriteFileResult* result =
        prepareVectorToStoreData<WriteFileResult>(ret_data);

    DWORD written = 0;

    const BOOL write_result =
        WriteFile(twfd->handle, twfd->buffer_is_null ? nullptr : twfd->buffer,
                  twfd->num_bytes_to_write, &written, NULL);

    result->success = (TRUE == write_result);
    result->last_error = GetLastError();
    result->bytes_written = written;

    return true;
  }
};

ICommand& get_write_file_command() {
  static WriteFileCommand command;
  return command;
}
