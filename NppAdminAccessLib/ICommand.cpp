#include "ICommand.hpp"

#include "CommandManager.hpp"

ICommand::ICommand(IWinApiFunctions& original_functions)
    : m_winapi(original_functions) {}

ICommand::~ICommand() = default;
