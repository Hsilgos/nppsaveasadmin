#pragma once

#include "ICommand.hpp"

std::unique_ptr<ICommand> make_create_filew_command(IWinApiFunctions& winapi);
