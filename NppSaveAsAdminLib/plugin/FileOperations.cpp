#include "FileOperations.h"

#include "Injection.h"

#include <list>
#include <memory>

FileFunctions &getFileFunctions()
{
  static FileFunctions tFuncs = { 0 };
  return tFuncs;
}


BOOL WINAPI MyWriteFileProc(
              HANDLE hFile,
              LPCVOID lpBuffer,
              DWORD nNumberOfBytesToWrite,
              LPDWORD lpNumberOfBytesWritten,
              LPOVERLAPPED lpOverlapped);

HANDLE WINAPI MyCreateFileA(
							LPCSTR lpFileName,
							DWORD dwDesiredAccess,
							DWORD dwShareMode,
							LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							DWORD dwCreationDisposition,
							DWORD dwFlagsAndAttributes,
							HANDLE hTemplateFile);

HANDLE WINAPI MyCreateFileW(
							LPCWSTR lpFileName,
							DWORD dwDesiredAccess,
							DWORD dwShareMode,
							LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							DWORD dwCreationDisposition,
							DWORD dwFlagsAndAttributes,
							HANDLE hTemplateFile);
BOOL WINAPI MyCloseHandle(HANDLE hObject);
DWORD WINAPI MyGetFileType(HANDLE aHandle);
/*
namespace
{
  std::list<std::unique_ptr<ScopedInjectorBase>> injectedFiles;
}

void doInjection()
{
  injectedFiles.emplace_back(inject("Kernel32.dll", "WriteFile", getFileFunctions().writeFile, &MyWriteFileProc));
  injectedFiles.emplace_back(inject("Kernel32.dll", "CreateFileA", getFileFunctions().createFileA, &MyCreateFileA));
  injectedFiles.emplace_back(inject("Kernel32.dll", "CreateFileW", getFileFunctions().createFileW, &MyCreateFileW));
  injectedFiles.emplace_back(inject("Kernel32.dll", "CloseHandle", getFileFunctions().closeHandle, &MyCloseHandle));
  injectedFiles.emplace_back(inject("Kernel32.dll", "GetFileType", getFileFunctions().getFileType, &MyGetFileType));
}

void unDoInjection()
{
 injectedFiles.clear();
}
*/