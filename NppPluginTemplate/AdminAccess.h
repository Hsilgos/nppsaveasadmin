#pragma once

#include <Windows.h>
#include <string>

void deleteAdminAccess();
HANDLE runAdminAccess(const std::wstring &aPipeName);

