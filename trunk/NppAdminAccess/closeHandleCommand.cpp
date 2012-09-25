#include "closeHandleCommand.h"

#include "common.h"

AUTOREG_COMMAND(CloseHandleCmd, CloseHandleCommand);

CloseHandleCommand::CloseHandleCommand(void)
{
}

CloseHandleCommand::~CloseHandleCommand(void)
{
}

bool CloseHandleCommand::execute( const void *aData, int aDataSize, void *aRetData, int &aRetDataSize )
{
	if( aDataSize != sizeof( CloseHandleData ) )
		return false;

	const CloseHandleData *tchd = static_cast<const CloseHandleData *>(aData);
	CloseHandleResult *tResult = static_cast<CloseHandleResult *>(aRetData);

	tResult->success = (TRUE == CloseHandle(tchd->handle));
	tResult->lastError = GetLastError();
	aRetDataSize = sizeof CloseHandleResult;

	return true;
}
