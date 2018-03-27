#include "ICommand.h"

class CloseHandleCommand : public ICommand {
  bool execute(const std::vector<char>& data,
               std::vector<char>& ret_data) override {
    const CloseHandleData* tchd =
        get_command_data_from_vector<CloseHandleData>(data);
    if (!tchd)
      return false;
    CloseHandleResult* result =
        prepareVectorToStoreData<CloseHandleResult>(ret_data);

    result->success = (TRUE == CloseHandle(tchd->handle));
    result->last_error = GetLastError();

    return true;
  }
};

ICommand& get_close_handle_command() {
  static CloseHandleCommand command;
  return command;
}
