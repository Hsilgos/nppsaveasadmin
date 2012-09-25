#pragma once

#include "iCommand.h"

class CreateFileACommand: public ICommand
{
public:
	virtual bool execute( const void *aData, int aDataSize, void *aRetData, int &aRetDataSize );
	CreateFileACommand();
	~CreateFileACommand();
};
