#pragma once

#include <unordered_map>

#include "common.h"

class ICommand;

class CommandManager {
  std::unordered_map<Commands, ICommand*> m_commands_map;

 public:
  void register_command(Commands cmd_num, ICommand* cmd);
  void erase_command(Commands cmd_num);
  ICommand* find_commad(Commands cmd_num);
};

void register_default_commands(CommandManager& manager);
