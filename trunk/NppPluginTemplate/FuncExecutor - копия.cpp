#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
#include "FuncExecutor.h"

#include "common.h"

#include "pipe.h"


template<class InData, class OutData>
bool ExecuteFunction(pipe &aPipe, const InData &aInData, Commands aCmdNum, OutData &aOutData)
{
	const int MaxBufferLen = 1024*4;
	char tInBuffer[MaxBufferLen];
	char tReadBuffer[MaxBufferLen];

	tInBuffer[0]		= aCmdNum;
	memcpy(&tInBuffer[1], &aInData, sizeof InData);
	DWORD tWritten		= 0;
	//BOOL tWriteResult	= getFileFunctions().writeFile(aPipe, tInBuffer, sizeof InData + 1, &tWritten, NULL);
	if( !aPipe.write(tInBuffer, sizeof InData + 1) )
		return false;

	DWORD tReadBytes	= 0;
	bool tReadResult	= aPipe.read(tReadBuffer, MaxBufferLen, tReadBytes);//ReadFile(aPipe, tReadBuffer, MaxBufferLen, &tReadBytes, NULL);
	OutData *tResult	= reinterpret_cast<OutData *>(tReadBuffer);
	memcpy(&aOutData, tResult, sizeof OutData);

	return tReadResult;
}


HANDLE ExecuteCreateFileW(
								 pipe &aPipe,
								 LPCWSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes)
{
	CreateFileDataW tcfdw		= {0};
	wcscpy(tcfdw.filename,	lpFileName );
	tcfdw.desiredAccess			= dwDesiredAccess;
	tcfdw.shareMode				= dwShareMode;
	tcfdw.flagsAndAttributes	= dwFlagsAndAttributes;
	tcfdw.creationDisposition	= dwCreationDisposition;
	CreateFileResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tcfdw, CreateFileWCmd, tResult) )
		return INVALID_HANDLE_VALUE;

	SetLastError(tResult.lastError);

	return tResult.handle;
}

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten)
{
	const char *tPtr = static_cast<const char *>(lpBuffer);

	DWORD tWritten = 0;

	while( nNumberOfBytesToWrite > 0 )
	{
		WriteFileData twfd			= {0};
		twfd.handle					= aOrigHandle;
		twfd.numBytesToWrite		= min(WriteFileData::MaxBufferSize, nNumberOfBytesToWrite);
		memcpy(twfd.buffer, tPtr, twfd.numBytesToWrite);

		WriteFileResult tResult		= {0};
		if( !ExecuteFunction(aPipe, twfd, WriteFileCmd, tResult) )
		{
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tWritten += tResult.bytesWritten;

		if( !tResult.success )
		{
			SetLastError(tResult.lastError);
			*lpNumberOfBytesWritten = tWritten;
			return FALSE;
		}

		tPtr += tResult.bytesWritten;
		nNumberOfBytesToWrite -= twfd.numBytesToWrite;
	}

	*lpNumberOfBytesWritten = tWritten;

	return TRUE;
}

DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle)
{
	GetFileTypeData tgft		= {0};
	tgft.handle					= aHandle;
	GetFileTypeResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tgft, GetFileTypeCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.type;
}

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle)
{
	CloseHandleData tchd		= {0};
	tchd.handle					= aHandle;
	CloseHandleResult tResult	= {0};
	if( !ExecuteFunction(aPipe, tchd, CloseHandleCmd, tResult) )
		return FALSE;

	SetLastError(tResult.lastError);

	return tResult.success;
}

void ExecuteExit(pipe &aPipe)
{
	char tCode			= ExitCmd;
	DWORD tWritten		= 0;
	BOOL tWriteResult	= aPipe.write(&tCode, 1);
}
