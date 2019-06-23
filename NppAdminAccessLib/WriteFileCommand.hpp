#pragma once

#include "ICommand.hpp"

std::unique_ptr<ICommand> make_write_file_command(IWinApiFunctions& winapi);
