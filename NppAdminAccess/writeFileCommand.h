#pragma once

#include "iCommand.h"

class WriteFileCommand: public ICommand
{
public:
	virtual bool execute( const void *aData, int aDataSize, void *aRetData, int &aRetDataSize );
	WriteFileCommand();
	~WriteFileCommand();
};
