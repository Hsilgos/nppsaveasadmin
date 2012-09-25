#include "createFileACommand.h"

#include "common.h"

AUTOREG_COMMAND(CreateFileACmd, CreateFileACommand);

CreateFileACommand::CreateFileACommand()
{
}

CreateFileACommand::~CreateFileACommand()
{
}

bool CreateFileACommand::execute( const void *aData, int aDataSize, void *aRetData, int &aRetDataSize )
{
	if( aDataSize != sizeof( CreateFileDataA ) )
		return false;

	const CreateFileDataA *tofs = static_cast<const CreateFileDataA *>(aData);
	CreateFileResult *tResult = static_cast<CreateFileResult *>(aRetData);

	SECURITY_ATTRIBUTES tSec = {0};
	tSec.nLength = sizeof(tSec);
	tSec.bInheritHandle = TRUE;

	HANDLE tHandle = CreateFileA(
		tofs->filename, 
		tofs->desiredAccess, 
		tofs->shareMode,
		&tSec,
		tofs->creationDisposition,
		tofs->flagsAndAttributes,
		0);

	tResult->success	= (tHandle != INVALID_HANDLE_VALUE);
	tResult->lastError	= GetLastError();
	if( tResult->success )
	{
		tResult->handle = tHandle;
	}

	aRetDataSize = sizeof(CreateFileResult);

	return true;
}

