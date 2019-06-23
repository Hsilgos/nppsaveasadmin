#include "ICommand.hpp"
#include "IWinApiFunctions.hpp"

class GetFileTypeCommand : public ICommand {
 public:
  using ICommand::ICommand;

 private:
  bool execute(const std::vector<char>& data,
               std::vector<char>& ret_data) override {
    const GetFileTypeData* tgft =
        get_command_data_from_vector<GetFileTypeData>(data);
    if (!tgft)
      return false;
    GetFileTypeResult* result =
        prepare_vector_to_store_data<GetFileTypeResult>(ret_data);

    result->type = m_winapi.get_file_type(tgft->handle);
    result->last_error = GetLastError();

    return true;
  }
};

std::unique_ptr<ICommand> make_file_type_command(
    IWinApiFunctions& original_functions) {
  return std::make_unique<GetFileTypeCommand>(original_functions);
}
