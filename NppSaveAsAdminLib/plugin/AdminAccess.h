#pragma once

#include <Windows.h>
#include <string>

void delete_admin_access();
HANDLE run_admin_access_app(const std::wstring& pipe_sender_name,
                            const std::wstring& pipe_receiver_name);
