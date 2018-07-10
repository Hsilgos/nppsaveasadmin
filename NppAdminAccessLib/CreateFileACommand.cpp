#include "ICommand.hpp"
#include "IWinApiFunctions.hpp"

class CreateFileACommand : public ICommand {
 public:
  using ICommand::ICommand;

 private:
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

    HANDLE handle = m_winapi.create_file_a(
        tofs->filename, tofs->desired_access, tofs->share_mode,
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

std::unique_ptr<ICommand> make_create_filea_command(
    IWinApiFunctions& original_functions) {
  return std::make_unique<CreateFileACommand>(original_functions);
}