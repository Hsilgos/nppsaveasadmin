#pragma once

#include "iCommand.h"

class GetFileTypeCommand: public ICommand
{
public:
	virtual bool execute( const void *aData, int aDataSize, void *aRetData, int &aRetDataSize );
	GetFileTypeCommand();
	~GetFileTypeCommand();
};
