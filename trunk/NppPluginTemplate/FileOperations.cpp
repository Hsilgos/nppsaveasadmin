#include "FileOperations.h"


FileFunctions &getFileFunctions()
{
	static FileFunctions tFuncs = {0};
	return tFuncs;
}


void AppendNewPtrImpl(PROC *aPtrToFunMemory, PROC aPtrToNewFun)
{
	MEMORY_BASIC_INFORMATION mbi = {0};
	VirtualQuery( aPtrToFunMemory, &mbi, sizeof(MEMORY_BASIC_INFORMATION) );
	VirtualProtect( mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);

	// Replace the original address of API with the address of corresponding 
	// wrapper function 
	*aPtrToFunMemory = *aPtrToNewFun;

	DWORD dwOldProtect = 0;
	VirtualProtect( mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &dwOldProtect );
}


void AppendNewPtrImpl(PROC *aPtrToFunMemory, PROC aPtrToNewFun);

template<class Proc>
void AppendNewPtr(PROC *aPtrToFunMemory, Proc aPtrToNewFun)
{
	AppendNewPtrImpl(aPtrToFunMemory, (PROC)aPtrToNewFun);
};

template<class Proc>
void doInjection(const WCHAR *aModuleName, const char *aFunctionName, Proc &aSaveOldToPtr, Proc aPtrToNewFun)
{
	const size_t tNameLen = strnlen(aFunctionName, 64);
	HMODULE hModule = GetModuleHandle(aModuleName);

	if( hModule )
	{
		ULONG ulSize = 0;

		PIMAGE_IMPORT_DESCRIPTOR pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)
			ImageDirectoryEntryToData(hModule, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

		if( pImportDesc )
		{
			while (pImportDesc->Name)
			{
				PIMAGE_THUNK_DATA pThunk =		(PIMAGE_THUNK_DATA)( (PBYTE) hModule + pImportDesc->FirstThunk );
				PIMAGE_THUNK_DATA pThunkOrig =	(PIMAGE_THUNK_DATA)( (PBYTE) hModule + pImportDesc->OriginalFirstThunk );

				while (pThunk->u1.AddressOfData)
				{
					PROC* ppfn = (PROC*) &pThunk->u1.AddressOfData;

					PIMAGE_IMPORT_BY_NAME pName =  (PIMAGE_IMPORT_BY_NAME)( (DWORD)hModule + pThunkOrig->u1.AddressOfData);

					if( 0 == strncmp(aFunctionName, (const char *)pName->Name, tNameLen) )
					{
						aSaveOldToPtr = (Proc)(PROC)*ppfn;
						AppendNewPtr(ppfn, aPtrToNewFun);

						return;
					}

					pThunk++;
					pThunkOrig++;
				}

				pImportDesc++;
			}
		}
	}
}

BOOL WINAPI MyWriteFileProc(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
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


void doInjection()
{
	doInjection(TEXT("Kernel32.dll"), "WriteFile",		getFileFunctions().writeFile,	&MyWriteFileProc);
	doInjection(TEXT("Kernel32.dll"), "CreateFileA",	getFileFunctions().createFileA,	&MyCreateFileA);
	doInjection(TEXT("Kernel32.dll"), "CreateFileW",	getFileFunctions().createFileW,	&MyCreateFileW);
	doInjection(TEXT("Kernel32.dll"), "CloseHandle",	getFileFunctions().closeHandle,	&MyCloseHandle);
	doInjection(TEXT("Kernel32.dll"), "GetFileType",	getFileFunctions().getFileType,	&MyGetFileType);
}


void unDoInjection()
{
	FileFunctions tDummy;

	doInjection(TEXT("Kernel32.dll"), "WriteFile",		tDummy.writeFile,	getFileFunctions().writeFile);
	doInjection(TEXT("Kernel32.dll"), "CreateFileA",	tDummy.createFileA,	getFileFunctions().createFileA);
	doInjection(TEXT("Kernel32.dll"), "CreateFileW",	tDummy.createFileW,	getFileFunctions().createFileW);
	doInjection(TEXT("Kernel32.dll"), "CloseHandle",	tDummy.closeHandle,	getFileFunctions().closeHandle);
	doInjection(TEXT("Kernel32.dll"), "GetFileType",	tDummy.getFileType,	getFileFunctions().getFileType);
}

