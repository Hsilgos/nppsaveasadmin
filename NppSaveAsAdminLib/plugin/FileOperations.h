#pragma once

// clang-format off
#include <Windows.h>
#include <Dbghelp.h>
#include <Psapi.h>
// clang-format on

#include "Injection.h"

typedef WINBASEAPI BOOL(WINAPI* WriteFileProc)(HANDLE hFile,
                                               LPCVOID lpBuffer,
                                               DWORD nNumberOfBytesToWrite,
                                               LPDWORD lpNumberOfBytesWritten,
                                               LPOVERLAPPED lpOverlapped);

typedef WINBASEAPI HANDLE(WINAPI* CreateFileAProc)(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile);

typedef WINBASEAPI HANDLE(WINAPI* CreateFileWProc)(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile);

typedef WINBASEAPI BOOL(WINAPI* CloseHandleProc)(HANDLE hObject);

typedef WINBASEAPI DWORD(WINAPI* GetFileTypeProc)(HANDLE hFile);

struct FileFunctions {
  injection_ptr_type(WriteFile) write_file;
  injection_ptr_type(CreateFileA) create_filea;
  injection_ptr_type(CreateFileW) create_filew;
  injection_ptr_type(GetFileType) get_file_type;
  injection_ptr_type(CloseHandle) close_handle;
};

/*
FileFunctions &getFileFunctions();

void unDoInjection();
void doInjection();
*/
