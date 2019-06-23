#pragma once

#include <Windows.h>
#include <string>

class IWinApiFunctions;

HANDLE run_admin_access_app(const std::wstring& exe_path,
						    const std::wstring& pipe_sender_name,
                            const std::wstring& pipe_receiver_name);

bool file_exists(const std::wstring& file);
