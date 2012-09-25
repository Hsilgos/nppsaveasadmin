#include "main.h"

#include <Windows.h>

#include <string>

#include "common.h"
#include "pipe.h"
#include "commandManager.h"
#include "iCommand.h"

Commands getCommand(const char *aBuffer);
//bool executeCommand(Commands aCmd, const void *aBuffer, int aDataSize);

int WINAPI CALLBACK WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	int tArgCount = 0;
	LPWSTR *tArgs = CommandLineToArgvW(GetCommandLineW(), &tArgCount);

	if( tArgCount < 2 )
	{
		MessageBox(0, TEXT("Do not run this application directly"), TEXT("Error"), MB_OK);
		return WrongExecute;
	}

	pipe tPipe;

	if ( !tPipe.open(tArgs[1]) )
		return FailedToOpenPipe;


	// The read operation will block until there is data to read
	const int MaxPipeSize = 4 * 1024;
	char tBuffer[MaxPipeSize];
	char tResultBuffer[MaxPipeSize];

	DWORD tReadSize = 0;

	CommandManager tMgr;

	while( tPipe.read(tBuffer, MaxPipeSize, tReadSize) )
	{
		if( tReadSize > 0 )
		{
			Commands tCmdNum = getCommand(tBuffer);
			if( ExitCmd == tCmdNum )
				break;

			ICommand *tCmd = tMgr.findCommad(tCmdNum);
			if( !tCmd )
				return UnknownCommand;

			int tResultSize = 0;
			bool tExecuted = tCmd->execute(tBuffer + 1, tReadSize - 1, tResultBuffer, tResultSize);
			if( !tExecuted )
				return BadDataInBuffer;
			
			if( !tPipe.write(tResultBuffer, tResultSize) )
				return FailedToWritePipe;
		}

		tReadSize = 0;
	}

	return NoError;
}

/*std::wstring fetchString(const void *aBuffer, int aDataSize)
{
	const wchar_t *tMBStr = static_cast<const wchar_t *>(aBuffer);

	const size_t tMaxSize = aDataSize/sizeof(wchar_t);

	const size_t tRealSize = wcsnlen_s(tMBStr, tMaxSize);

	if( tRealSize == tMaxSize )
		return L"";

	return std::wstring(tMBStr);
}
*/
Commands getCommand(const char *aBuffer)
{
	if( aBuffer[0] <= UnknownCmd || aBuffer[0] >= MaxCmd )
		return UnknownCmd;

	return static_cast<Commands>(aBuffer[0]);
}
/*
bool executeCommand(Commands aCmd, const void *aBuffer, int aDataSize)
{
	switch ( aCmd )
	{
	case CreateFileACmd:
		return true;
	case CreateFileWCmd:
		return true;
	}

	return false;
}*/



