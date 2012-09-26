#include "AdminAccess.h"

#include <fstream>
#include <Windows.h>

#include "generated/NppAdminAccess.h"

#include "FileOperations.h"

std::wstring getAdminAccessPath()
{
	wchar_t tTempPath[MAX_PATH] = {0};
	DWORD tLen = GetTempPathW(MAX_PATH, tTempPath);
	if( tLen == 0 )
		return false;
	std::wstring tPathStr(tTempPath, tLen);
	tPathStr += L"NppAdminAccess.exe";

	return tPathStr;
}


void deleteAdminAccess()
{
	DeleteFileW( getAdminAccessPath().c_str() );
}

bool dumpAdminAccess(std::wstring &aPath)
{

	aPath = getAdminAccessPath();

	//std::wofstream tFile(tTempPath, std::wofstream::binary);
	//tFile.write(get_NppAdminAccess_buf(), get_NppAdminAccess_size());
	const HANDLE thFile = getFileFunctions().createFileW(
		aPath.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	
	if( INVALID_HANDLE_VALUE == thFile )
		return ERROR_FILE_EXISTS == GetLastError();

	DWORD tWritten = 0;

	const BOOL tWriteRes = getFileFunctions().writeFile(
		thFile, 
		get_NppAdminAccess_buf(), 
		get_NppAdminAccess_size(),
		&tWritten,
		NULL);

	getFileFunctions().closeHandle(thFile);

	return TRUE == tWriteRes;
}

HANDLE runAdminAccess(const std::wstring &aExePath, const std::wstring &aPipeName)
{
	SHELLEXECUTEINFOW sinfo = {0};

	sinfo.cbSize       = sizeof SHELLEXECUTEINFOW;
	sinfo.fMask        = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_NOCLOSEPROCESS ;
	sinfo.hwnd         = NULL;				//
	sinfo.lpFile       = aExePath.c_str();	//
	sinfo.lpParameters = aPipeName.c_str(); //L"\\\\.\\pipe\\my_pipe";
	sinfo.lpVerb       = L"runas";			// <<-- this is what makes a UAC prompt show up
	sinfo.nShow        = SW_SHOWMAXIMIZED;	//

	if( TRUE != ShellExecuteExW(&sinfo) )
		return NULL;

	return sinfo.hProcess;
}

HANDLE runAdminAccess(const std::wstring &aPipeName)
{
	std::wstring tExePath;
	if( !dumpAdminAccess(tExePath) )
		return NULL;

	return runAdminAccess(tExePath, aPipeName);
}

