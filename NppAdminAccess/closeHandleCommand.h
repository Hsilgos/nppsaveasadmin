#pragma once

#include "iCommand.h"


class CloseHandleCommand: public ICommand
{
public:
	virtual bool execute( const void *aData, int aDataSize, void *aRetData, int &aRetDataSize );
	CloseHandleCommand(void);
	~CloseHandleCommand(void);
};
