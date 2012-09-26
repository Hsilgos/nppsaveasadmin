#pragma once

#include <Windows.h>

enum Errors
{
	NoError = 0,
	WrongExecute,
	FailedToOpenPipe,
	ErrorCommandExecution,
	UnknownCommand,
	BadDataInBuffer,
	FailedToWritePipe,
	ReturnBufferHasNoData
};

// commands
enum Commands
{
	UnknownCmd	= 0,
	
	CreateFileACmd,
	CreateFileWCmd,
	WriteFileCmd,
	CloseHandleCmd,
	GetFileTypeCmd,
	ExitCmd,

	MaxCmd
};

// CreateFile
struct CreateFileDataA
{
	char filename[1024];
	DWORD desiredAccess;
	DWORD shareMode;
	DWORD creationDisposition;
	DWORD flagsAndAttributes;
};

struct CreateFileDataW
{
	wchar_t filename[1024];
	DWORD desiredAccess;
	DWORD shareMode;
	DWORD creationDisposition;
	DWORD flagsAndAttributes;
};

struct CreateFileResult
{
	bool	success;
	HANDLE	handle;
	DWORD	lastError;
};

// WriteFile
struct WriteFileData
{
	HANDLE	handle;
	DWORD	numBytesToWrite;
	enum 
	{
		MaxBufferSize = 1024*3
	};
	char	buffer[MaxBufferSize];
};

struct WriteFileResult
{
	bool	success;
	DWORD	bytesWritten;
	DWORD	lastError;
};

// CloseHandle
struct CloseHandleData
{
	HANDLE handle;
};

struct CloseHandleResult
{
	bool	success;
	DWORD	lastError;
};

// GetFileType
struct GetFileTypeData
{
	HANDLE handle;
};

struct GetFileTypeResult
{
	DWORD type;
	int lastError;
};
