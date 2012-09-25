#pragma once

#include "common.h"
#include <map>

class ICommand;

class CommandManager
{
	static std::map<Commands, ICommand*> &getCommands();
public:
	CommandManager();
	~CommandManager();

	static void registerCommand(Commands aCmdNum, ICommand* aCmd);
	ICommand* findCommad(Commands aCmdNum);
};
