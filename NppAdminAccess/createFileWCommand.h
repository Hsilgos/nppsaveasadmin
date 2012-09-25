#pragma once

#include "iCommand.h"

class CreateFileWCommand: public ICommand
{
public:
	virtual bool execute( const void *aData, int aDataSize, void *aRetData, int &aRetDataSize );
	CreateFileWCommand();
	~CreateFileWCommand();
};
