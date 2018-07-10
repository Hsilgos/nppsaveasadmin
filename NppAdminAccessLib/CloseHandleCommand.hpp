#pragma once

#include "ICommand.hpp"

std::unique_ptr<ICommand> make_close_handle_command(
    IWinApiFunctions& winapi);
