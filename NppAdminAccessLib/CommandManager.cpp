#include "CommandManager.h"

#include "CloseHandleCommand.h"
#include "CreateFileACommand.h"
#include "CreateFileWCommand.h"
#include "GetFileTypeCommand.h"
#include "WriteFileCommand.h"

#include <unordered_map>

void CommandManager::register_command(Commands cmd_num, ICommand* cmd) {
  m_commands_map[cmd_num] = cmd;
}

void CommandManager::erase_command(Commands cmd_num) {
  m_commands_map.erase(cmd_num);
}

ICommand* CommandManager::find_commad(Commands cmd_num) {
  auto it = m_commands_map.find(cmd_num);
  if (it != m_commands_map.end())
    return it->second;

  return nullptr;
}

void register_default_commands(CommandManager& manager) {
  manager.register_command(CloseHandleCmd, &get_close_handle_command());
  manager.register_command(CreateFileACmd, &get_create_filea_command());
  manager.register_command(CreateFileWCmd, &get_create_filew_command());
  manager.register_command(WriteFileCmd, &get_write_file_command());
  manager.register_command(GetFileTypeCmd, &get_file_type_command());
}
