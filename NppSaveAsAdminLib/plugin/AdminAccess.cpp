#include "AdminAccess.h"

#include <fstream>
#include <Windows.h>

#include "generated/NppAdminAccess.h"

#include "FileOperations.h"

std::wstring get_admin_access_path()
{
	wchar_t temp_path[MAX_PATH] = {0};
	DWORD path_length = GetTempPathW(MAX_PATH, temp_path);
	if( path_length == 0 )
		return std::wstring();
	std::wstring path_string(temp_path, path_length);
	path_string += L"NppAdminAccess.exe";

	return path_string;
}

FileFunctions &getFileFunctions();

void delete_admin_access()
{
	DeleteFileW( get_admin_access_path().c_str() );
}

bool dump_admin_access(std::wstring &path)
{
	path = get_admin_access_path();

	const HANDLE file = getFileFunctions().create_filew->call_original(
		path.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	
	if( INVALID_HANDLE_VALUE == file )
		return ERROR_FILE_EXISTS == GetLastError();

	DWORD written = 0;

	const BOOL write_result = getFileFunctions().write_file->call_original(
		file, 
		get_NppAdminAccess_buf(), 
		get_NppAdminAccess_size(),
		&written,
		NULL);

	getFileFunctions().close_handle->call_original(file);

	return TRUE == write_result;
}

HANDLE run_admin_access(
  const std::wstring &exe_path,
  const std::wstring & pipe_sender_name,
  const std::wstring & pipe_receiver_name)
{
	SHELLEXECUTEINFOW sinfo = {0};

  std::wstring params;
  params.append(L"\"").append(pipe_sender_name).append(L"\" ")
    .append(L"\"").append(pipe_receiver_name).append(L"\"");

	sinfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	sinfo.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_NOCLOSEPROCESS ;
	sinfo.hwnd = NULL;
	sinfo.lpFile = exe_path.c_str();
	sinfo.lpParameters = params.c_str(); //L"\\\\.\\pipe\\my_pipe";
	sinfo.lpVerb = L"runas";			    // <<-- this is what makes a UAC prompt show up
	sinfo.nShow = SW_SHOWMAXIMIZED;	//

	if( TRUE != ShellExecuteExW(&sinfo) )
		return NULL;

	return sinfo.hProcess;
}

HANDLE run_admin_access_app(const std::wstring & pipe_sender_name,
  const std::wstring & pipe_receiver_name)
{
  std::wstring exe_path;
  if (!dump_admin_access(exe_path))
    return NULL;

  return run_admin_access(exe_path, pipe_sender_name, pipe_receiver_name);
}
