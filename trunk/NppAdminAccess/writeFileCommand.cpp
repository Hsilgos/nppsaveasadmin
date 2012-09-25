#include "writeFileCommand.h"

#include "common.h"

AUTOREG_COMMAND(WriteFileCmd, WriteFileCommand);

WriteFileCommand::WriteFileCommand()
{
}

WriteFileCommand::~WriteFileCommand()
{
}

bool WriteFileCommand::execute( const void *aData, int aDataSize, void *aRetData, int &aRetDataSize )
{
	if( aDataSize != sizeof( WriteFileData ) )
		return false;

	const WriteFileData *twfd = static_cast<const WriteFileData *>(aData);
	WriteFileResult *tResult = static_cast<WriteFileResult *>(aRetData);

	DWORD tWritten = 0;

	const BOOL tWriteResult = WriteFile(
		twfd->handle, 
		twfd->buffer,
		twfd->numBytesToWrite,
		&tWritten,
		NULL);

	tResult->success	= (TRUE == tWriteResult);
	tResult->lastError	= GetLastError();

	if( tResult->success )
	{
		tResult->bytesWritten = tWritten;
	}

	aRetDataSize = sizeof(WriteFileResult);

	return true;
}

