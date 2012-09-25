#pragma once

#include "common.h"

class ICommand
{
public:
	virtual bool execute( const void *aData, int aDataSize, void *aRetData, int &aRetDataSize ) = 0;
	ICommand();
	virtual ~ICommand();
};

struct AutoRegCommand
{
	AutoRegCommand(Commands aCmdNum, ICommand *aCmd);
};

#define AUTOREG_COMMAND( cmd_num, icommand_child_type ) \
	static icommand_child_type g_static_command_##icommand_child_type;		\
	static AutoRegCommand g_static_command_register_##icommand_child_type_##__LINE__(cmd_num, &g_static_command_##icommand_child_type);

