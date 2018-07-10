#pragma once

#include <memory>
#include <unordered_map>

#include "common.hpp"

class ICommand;
class IWinApiFunctions;

class CommandManager {
  std::unordered_map<Commands, std::shared_ptr<ICommand>> m_commands_map;

 public:
  CommandManager();
  ~CommandManager();
  void register_command(Commands cmd_num, std::shared_ptr<ICommand> cmd);
  void erase_command(Commands cmd_num);
  ICommand* find_commad(Commands cmd_num);
};

void register_default_commands(CommandManager& manager, IWinApiFunctions& original_functions);
