#include "CommandManager.hpp"

#include "CloseHandleCommand.hpp"
#include "CreateFileWCommand.hpp"
#include "GetFileTypeCommand.hpp"
#include "WriteFileCommand.hpp"

#include <unordered_map>

CommandManager::CommandManager() = default;

CommandManager::~CommandManager() = default;

void CommandManager::register_command(Commands cmd_num,
                                      std::shared_ptr<ICommand> cmd) {
  m_commands_map[cmd_num] = std::move(cmd);
}

void CommandManager::erase_command(Commands cmd_num) {
  m_commands_map.erase(cmd_num);
}

ICommand* CommandManager::find_commad(Commands cmd_num) {
  auto it = m_commands_map.find(cmd_num);
  if (it != m_commands_map.end())
    return it->second.get();

  return nullptr;
}

void register_default_commands(CommandManager& manager,
                               IWinApiFunctions& winapi) {
  manager.register_command(CloseHandleCmd, make_close_handle_command(winapi));
  manager.register_command(CreateFileWCmd, make_create_filew_command(winapi));
  manager.register_command(WriteFileCmd, make_write_file_command(winapi));
  manager.register_command(GetFileTypeCmd, make_file_type_command(winapi));
}
