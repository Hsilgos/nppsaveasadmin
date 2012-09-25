//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"

#include <map>
#include <set>
#include <string>

#include "FileOperations.h"
#include "AdminAccess.h"
#include "FuncExecutor.h"

#include "pipe.h"

extern FuncItem funcItem[nbFunc];
extern NppData nppData;

struct MyFileHandle
{
	pipe pipe;
	HANDLE procHandle;
	HANDLE originHandle;
};


typedef std::map<HANDLE, MyFileHandle *> HandleMap;
HandleMap myHandles;
std::set<std::wstring> allowedFiles;

//////////////////////////////////////////////////////////////////////////

BOOL WINAPI MyWriteFileProc(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	HandleMap::iterator it = myHandles.find(hFile);
	if( it != myHandles.end() )
	{
		return ExecuteWriteFile(
			it->second->pipe, 
			it->second->originHandle, 
			lpBuffer, 
			nNumberOfBytesToWrite, 
			lpNumberOfBytesWritten);
	}
	else
	{
		return getFileFunctions().writeFile(
			hFile, 
			lpBuffer, 
			nNumberOfBytesToWrite, 
			lpNumberOfBytesWritten, 
			lpOverlapped);
	}
}

HANDLE WINAPI MyCreateFileA(
							LPCSTR lpFileName,
							DWORD dwDesiredAccess,
							DWORD dwShareMode,
							LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							DWORD dwCreationDisposition,
							DWORD dwFlagsAndAttributes,
							HANDLE hTemplateFile)
{
	return getFileFunctions().createFileA(
		lpFileName, 
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

HANDLE WINAPI MyCreateFileW(
							 LPCWSTR lpFileName,
							 DWORD dwDesiredAccess,
							 DWORD dwShareMode,
							 LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							 DWORD dwCreationDisposition,
							 DWORD dwFlagsAndAttributes,
							 HANDLE hTemplateFile)
{
	HANDLE tResult = getFileFunctions().createFileW(
		lpFileName, 
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);

	if( INVALID_HANDLE_VALUE == tResult && (dwDesiredAccess & GENERIC_WRITE) )
	{
		const int tErrorCode = GetLastError();
		if( ERROR_ACCESS_DENIED == tErrorCode && 
			allowedFiles.find(lpFileName) != allowedFiles.end() )
		{
			MyFileHandle *tNewHandle = new MyFileHandle;
			if( !tNewHandle->pipe.createUnique() )
			{
				delete tNewHandle;
				return INVALID_HANDLE_VALUE;
			}

			tNewHandle->procHandle = runAdminAccess( tNewHandle->pipe.getName() );
			if( NULL == tNewHandle->procHandle )
			{
				delete tNewHandle;
				return INVALID_HANDLE_VALUE;
			}

			tNewHandle->pipe.wait();

			tNewHandle->originHandle = ExecuteCreateFileW(
				tNewHandle->pipe, 
				lpFileName, 
				dwDesiredAccess,
				dwShareMode, 
				dwCreationDisposition,
				dwFlagsAndAttributes);
			if( INVALID_HANDLE_VALUE == tNewHandle->originHandle )
			{
				delete tNewHandle;
				return INVALID_HANDLE_VALUE;
			}

			myHandles[tNewHandle->procHandle] = tNewHandle;
			tResult = tNewHandle->procHandle;
		}
		
	}

	return tResult;
}

BOOL WINAPI MyCloseHandle(HANDLE hObject)
{
	HandleMap::iterator it = myHandles.find(hObject);
	if( it != myHandles.end() )
	{
		BOOL tResult = ExecuteCloseHandle(it->second->pipe, it->second->originHandle);
		ExecuteExit(it->second->pipe);
		delete it->second;
		myHandles.erase(it);
		return tResult;
	}
	else
	{
		return getFileFunctions().closeHandle(hObject);
	}
}

DWORD WINAPI MyGetFileType(HANDLE aHandle)
{
	HandleMap::iterator it = myHandles.find(aHandle);
	if( it != myHandles.end() )
	{
		return FILE_TYPE_DISK;
	}
	else
	{
		return getFileFunctions().getFileType(aHandle);
	}
}
//////////////////////////////////////////////////////////////////////////

/*
void doInjection()
{
	doInjection(TEXT("Kernel32.dll"), "WriteFile",		getFileFunctions().writeFile,	&MyWriteFileProc);
	doInjection(TEXT("Kernel32.dll"), "CreateFileA",	getFileFunctions().createFileA,	&MyCreateFileA);
	doInjection(TEXT("Kernel32.dll"), "CreateFileW",	getFileFunctions().createFileW,	&MyCreateFileW);
	doInjection(TEXT("Kernel32.dll"), "CloseHandle",	getFileFunctions().closeHandle,	&MyCloseHandle);
	doInjection(TEXT("Kernel32.dll"), "GetFileType",	getFileFunctions().getFileType,	&MyGetFileType);
}*/


/*
void unDoInjection()
{
	FileFunctions tDummy;

	doInjection(TEXT("Kernel32.dll"), "WriteFile",		tDummy.writeFile,	getFileFunctions().writeFile);
	doInjection(TEXT("Kernel32.dll"), "CreateFileA",	tDummy.createFileA,	getFileFunctions().createFileA);
	doInjection(TEXT("Kernel32.dll"), "CreateFileW",	tDummy.createFileW,	getFileFunctions().createFileW);
	doInjection(TEXT("Kernel32.dll"), "CloseHandle",	tDummy.closeHandle,	getFileFunctions().closeHandle);
	doInjection(TEXT("Kernel32.dll"), "GetFileType",	tDummy.getFileType,	getFileFunctions().getFileType);
}*/



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  reasonForCall, 
                       LPVOID lpReserved )
{
    switch (reasonForCall)
    {
      case DLL_PROCESS_ATTACH:
        pluginInit(hModule);
		deleteAdminAccess();
		doInjection();
        break;

      case DLL_PROCESS_DETACH:
		commandMenuCleanUp();
        pluginCleanUp();
		deleteAdminAccess();
		unDoInjection();
        break;

      case DLL_THREAD_ATTACH:
        break;

      case DLL_THREAD_DETACH:
        break;
    }

    return TRUE;
}


extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	nppData = notpadPlusData;
	commandMenuInit();
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
	return NPP_PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	*nbF = nbFunc;
	return funcItem;
}


std::wstring currentFilePath(SCNotification *notifyCode)
{
	wchar_t *theFullPath;
	int avar = SendMessage(
		nppData._nppHandle,
		NPPM_GETFULLPATHFROMBUFFERID,
		(WPARAM)notifyCode->nmhdr.idFrom,
		(LPARAM)NULL
		);

	if( avar == 0 )
		return std::wstring();

	theFullPath = new wchar_t[avar + 1];
	theFullPath[0] = 0;
	avar = SendMessage(
		nppData._nppHandle,
		NPPM_GETFULLPATHFROMBUFFERID,
		(WPARAM)notifyCode->nmhdr.idFrom,
		(LPARAM)theFullPath
		);

	std::wstring tPath = theFullPath;

	delete []theFullPath;

	return tPath;
}

extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
	if( notifyCode->nmhdr.code == NPPN_FILEBEFORESAVE )
	{
		std::wstring tpath = currentFilePath(notifyCode);
		if( !tpath.empty() )
			allowedFiles.insert( tpath );
	}
	else if(notifyCode->nmhdr.code == NPPN_FILESAVED)
	{
		std::wstring tpath = currentFilePath(notifyCode);
		std::set<std::wstring>::iterator it = allowedFiles.find(tpath);
		if( it != allowedFiles.end() )
			allowedFiles.erase(it);
	}
}


// Here you can process the Npp Messages 
// I will make the messages accessible little by little, according to the need of plugin development.
// Please let me know if you need to access to some messages :
// http://sourceforge.net/forum/forum.php?forum_id=482781
//
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{/*
	if (Message == WM_MOVE)
	{
		::MessageBox(NULL, "move", "", MB_OK);
	}
*/
	return TRUE;
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
    return TRUE;
}
#endif //UNICODE
