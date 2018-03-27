#include "ICommand.h"

class GetFileTypeCommand : public ICommand {
  bool execute(const std::vector<char>& data,
               std::vector<char>& ret_data) override {
    const GetFileTypeData* tgft =
        get_command_data_from_vector<GetFileTypeData>(data);
    if (!tgft)
      return false;
    GetFileTypeResult* result =
        prepareVectorToStoreData<GetFileTypeResult>(ret_data);

    result->type = GetFileType(tgft->handle);
    result->last_error = GetLastError();

    return true;
  }
};

ICommand& get_file_type_command() {
  static GetFileTypeCommand command;
  return command;
}
