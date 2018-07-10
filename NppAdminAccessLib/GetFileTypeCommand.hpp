#pragma once

#include "ICommand.hpp"

std::unique_ptr<ICommand> make_file_type_command(
    IWinApiFunctions& winapi);
