#pragma once

#include "ICommand.hpp"

std::unique_ptr<ICommand> make_create_filea_command(
    IWinApiFunctions& winapi);
