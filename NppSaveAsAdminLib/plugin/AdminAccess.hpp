#pragma once

#include <Windows.h>
#include <string>

class IWinApiFunctions;

void delete_admin_access();
HANDLE run_admin_access_app(IWinApiFunctions& original_functions,
                            const std::wstring& pipe_sender_name,
                            const std::wstring& pipe_receiver_name);
