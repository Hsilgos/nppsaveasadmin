#include "iCommand.h"

#include "commandManager.h"

ICommand::ICommand()
{
}

ICommand::~ICommand()
{
}

AutoRegCommand::AutoRegCommand(Commands aCmdNum, ICommand *aCmd)
{
	CommandManager::registerCommand(aCmdNum, aCmd);
}

