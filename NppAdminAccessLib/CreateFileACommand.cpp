#include "ICommand.h"

class CreateFileACommand : public ICommand {
  bool execute(const std::vector<char>& data,
               std::vector<char>& ret_data) override {
    const CreateFileDataA* tofs =
        get_command_data_from_vector<CreateFileDataA>(data);
    if (!tofs)
      return false;
    CreateFileResult* result =
        prepareVectorToStoreData<CreateFileResult>(ret_data);

    SECURITY_ATTRIBUTES security_attributes = {0};
    security_attributes.nLength = sizeof(security_attributes);
    security_attributes.bInheritHandle = TRUE;

    HANDLE handle =
        CreateFileA(tofs->filename, tofs->desired_access, tofs->share_mode,
                    &security_attributes, tofs->creation_disposition,
                    tofs->flags_and_attributes, 0);

    result->success = (handle != INVALID_HANDLE_VALUE);
    result->last_error = GetLastError();
    if (result->success) {
      result->handle = handle;
    }

    return true;
  }
};

ICommand& get_create_filea_command() {
  static CreateFileACommand command;
  return command;
}
