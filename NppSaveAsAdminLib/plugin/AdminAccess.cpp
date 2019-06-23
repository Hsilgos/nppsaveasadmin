#include "AdminAccess.hpp"

#include <Windows.h>
#include <fstream>

#include "IWinApiFunctions.hpp"
#include "UniqueHandle.hpp"

HANDLE run_admin_access(const std::wstring& exe_path,
                        const std::wstring& pipe_sender_name,
                        const std::wstring& pipe_receiver_name) {
  SHELLEXECUTEINFOW sinfo = {0};

  std::wstring params;
  params.append(L"\"")
      .append(pipe_sender_name)
      .append(L"\" ")
      .append(L"\"")
      .append(pipe_receiver_name)
      .append(L"\"");

  sinfo.cbSize = sizeof(SHELLEXECUTEINFOW);
  sinfo.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_NOCLOSEPROCESS;
  sinfo.hwnd = NULL;
  sinfo.lpFile = exe_path.c_str();
  sinfo.lpParameters = params.c_str();  // L"\\\\.\\pipe\\my_pipe";
  sinfo.lpVerb = L"runas";  // <<-- this is what makes a UAC prompt show up
  sinfo.nShow = SW_SHOWMAXIMIZED;  //

  if (TRUE != ShellExecuteExW(&sinfo))
    return INVALID_HANDLE_VALUE;

  return sinfo.hProcess;
}

bool file_exists(const std::wstring& file) {
  const DWORD attributes = GetFileAttributes(file.c_str());

  return (attributes != INVALID_FILE_ATTRIBUTES &&
          !(attributes & FILE_ATTRIBUTE_DIRECTORY));
}

HANDLE run_admin_access_app(const std::wstring& exe_path,
                            const std::wstring& pipe_sender_name,
                            const std::wstring& pipe_receiver_name) {
  if (!file_exists(exe_path))
    return INVALID_HANDLE_VALUE;

  return run_admin_access(exe_path, pipe_sender_name, pipe_receiver_name);
}
