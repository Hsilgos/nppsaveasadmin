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
  ScopedInjector<BOOL, HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED>::Pointer
      write_file;
  ScopedInjector<HANDLE,
                 LPCSTR,
                 DWORD,
                 DWORD,
                 LPSECURITY_ATTRIBUTES,
                 DWORD,
                 DWORD,
                 HANDLE>::Pointer create_filea;
  ScopedInjector<HANDLE,
                 LPCWSTR,
                 DWORD,
                 DWORD,
                 LPSECURITY_ATTRIBUTES,
                 DWORD,
                 DWORD,
                 HANDLE>::Pointer create_filew;
  ScopedInjector<BOOL, HANDLE>::Pointer close_handle;
  ScopedInjector<DWORD, HANDLE>::Pointer get_file_type;
};

/*
FileFunctions &getFileFunctions();

void unDoInjection();
void doInjection();
*/
