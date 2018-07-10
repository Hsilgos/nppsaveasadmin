#pragma once

#include <string>

class CommandManager;

int process_commands(CommandManager& cmd_manager,
                     const std::wstring& pipe_name_to_read,
                     const std::wstring& pipe_name_to_write);
