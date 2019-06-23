#include "ICommand.hpp"
#include "IWinApiFunctions.hpp"

class CloseHandleCommand : public ICommand {
 public:
  using ICommand::ICommand;

 private:
  bool execute(const std::vector<char>& data,
               std::vector<char>& ret_data) override {
    const CloseHandleData* tchd =
        get_command_data_from_vector<CloseHandleData>(data);
    if (!tchd)
      return false;
    CloseHandleResult* result =
        prepare_vector_to_store_data<CloseHandleResult>(ret_data);

    result->success = (TRUE == m_winapi.close_handle(tchd->handle));
    result->last_error = GetLastError();

    return true;
  }
};

std::unique_ptr<ICommand> make_close_handle_command(IWinApiFunctions& winapi) {
  return std::make_unique<CloseHandleCommand>(winapi);
}
