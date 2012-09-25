#include "commandManager.h"

CommandManager::CommandManager()
{
}

CommandManager::~CommandManager()
{
}

std::map<Commands, ICommand*> &CommandManager::getCommands()
{
	static std::map<Commands, ICommand*> tMap;
	return tMap;
}

void CommandManager::registerCommand(Commands aCmdNum, ICommand* aCmd)
{
	getCommands()[aCmdNum] = aCmd;
}

ICommand* CommandManager::findCommad(Commands aCmdNum)
{
	std::map<Commands, ICommand*>::const_iterator it = getCommands().find(aCmdNum);
	if( it != getCommands().end() )
		return it->second;

	return NULL;
}
