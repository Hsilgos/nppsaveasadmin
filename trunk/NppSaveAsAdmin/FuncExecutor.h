#pragma once

#include <Windows.h>


class pipe;

HANDLE ExecuteCreateFileW(
							pipe &aPipe,
							LPCWSTR lpFileName,
							DWORD dwDesiredAccess,
							DWORD dwShareMode,
							DWORD dwCreationDisposition,
							DWORD dwFlagsAndAttributes);

BOOL ExecuteWriteFile(
							 pipe &aPipe, 
							 HANDLE aOrigHandle,
							 LPCVOID lpBuffer, 
							 DWORD nNumberOfBytesToWrite, 
							 LPDWORD lpNumberOfBytesWritten);
DWORD ExecuteGetFileType(pipe &aPipe, HANDLE aHandle);

BOOL ExecuteCloseHandle(pipe &aPipe, HANDLE aHandle);
void ExecuteExit(pipe &aPipe);
