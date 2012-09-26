#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <Dbghelp.h>

typedef WINBASEAPI BOOL (WINAPI *WriteFileProc)(HANDLE hFile,LPCVOID lpBuffer,DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten,LPOVERLAPPED lpOverlapped);

typedef WINBASEAPI HANDLE (WINAPI *CreateFileAProc)(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile);


typedef WINBASEAPI HANDLE (WINAPI *CreateFileWProc)(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile);


typedef WINBASEAPI BOOL (WINAPI *CloseHandleProc)(HANDLE hObject);

typedef WINBASEAPI DWORD(WINAPI *GetFileTypeProc)(HANDLE hFile);

struct FileFunctions
{
	WriteFileProc	writeFile;
	CreateFileAProc createFileA;
	CreateFileWProc createFileW;
	CloseHandleProc closeHandle;
	GetFileTypeProc	getFileType;
};

FileFunctions &getFileFunctions();

void unDoInjection();
void doInjection();

