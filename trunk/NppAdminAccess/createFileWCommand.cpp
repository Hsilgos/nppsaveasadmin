#include "createFileWCommand.h"

#include "common.h"

AUTOREG_COMMAND(CreateFileWCmd, CreateFileWCommand);

CreateFileWCommand::CreateFileWCommand()
{
}

CreateFileWCommand::~CreateFileWCommand()
{
}

bool CreateFileWCommand::execute( const void *aData, int aDataSize, void *aRetData, int &aRetDataSize )
{
	if( aDataSize != sizeof( CreateFileDataW ) )
		return false;

	const CreateFileDataW *tofs = static_cast<const CreateFileDataW *>(aData);
	CreateFileResult *tResult = static_cast<CreateFileResult *>(aRetData);

	SECURITY_ATTRIBUTES tSec = {0};
	tSec.nLength = sizeof(tSec);
	tSec.bInheritHandle = TRUE;

	HANDLE tHandle = CreateFileW(
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

