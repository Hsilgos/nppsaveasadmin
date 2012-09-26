#include "getFileTypeCommand.h"

#include "common.h"

AUTOREG_COMMAND(GetFileTypeCmd, GetFileTypeCommand);

GetFileTypeCommand::GetFileTypeCommand()
{
}

GetFileTypeCommand::~GetFileTypeCommand()
{
}

bool GetFileTypeCommand::execute( const void *aData, int aDataSize, void *aRetData, int &aRetDataSize )
{
	if( aDataSize != sizeof( GetFileTypeData ) )
		return false;

	const GetFileTypeData *tgft = static_cast<const GetFileTypeData *>(aData);
	GetFileTypeResult *tResult = static_cast<GetFileTypeResult *>(aRetData);


	tResult->type = GetFileType(tgft->handle);
	tResult->lastError = GetLastError();

	aRetDataSize = sizeof GetFileTypeResult;

	return true;
}

