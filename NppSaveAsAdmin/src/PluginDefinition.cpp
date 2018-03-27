// this file is part of notepad++
// Copyright (C)2003 Don HO <donho@altern.org>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "menuCmdID.h"

#include "plugin/FileOperations.h"

#include <sstream>
#include <vector>

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

std::wstring g_version;

static std::wstring get_path(HANDLE hModule) {
  TCHAR module_file[2048] = {0};
  const int result_size =
      GetModuleFileName((HMODULE)hModule, module_file, 2048);
  return std::wstring(module_file, result_size);
}

static std::wstring get_exe_version(const std::wstring& exe_file_name,
                                    const std::wstring& field_name) {
  DWORD pointless_win32_variable = 0;
  DWORD size =
      GetFileVersionInfoSizeW(exe_file_name.c_str(), &pointless_win32_variable);

  if (size > 0) {
    std::vector<char> exe_info;
    exe_info.resize(size);

    if (GetFileVersionInfoW(exe_file_name.c_str(), 0, size, &exe_info[0])) {
      wchar_t* result = 0;
      unsigned int result_length = 0;

      // try the 1200 codepage (Unicode)
      std::wstring query_string(L"\\StringFileInfo\\041904B0\\" + field_name);

      if (!VerQueryValueW(&exe_info[0], query_string.c_str(), (void**)&result,
                          &result_length)) {
        // try the 1252 codepage (Windows Multilingual)
        query_string = L"\\StringFileInfo\\041904E4\\" + field_name;
        VerQueryValueW(&exe_info[0], query_string.c_str(), (void**)&result,
                       &result_length);
      }

      if (result && result_length > 0)
        return std::wstring(result, result_length - 1);
    }
  }

  return std::wstring();
}

//
// Initialize your plugin data here
// It will be called while plugin loading
void pluginInit(HANDLE hModule) {
  g_version = get_exe_version(get_path(hModule), L"FileVersion");
}

//
// Here you can do the clean up, save the parameters (if any) for the next
// session
//
void pluginCleanUp() {}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit() {
  setCommand(0, TEXT("About"), about, NULL, false);

  if (is_debugging()) {
    // setCommand(1, TEXT("Hook"), do_injection, NULL, false);
    // setCommand(2, TEXT("Unhook"), un_do_injection, NULL, false);
  }
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp() {
  // Don't forget to deallocate your shortcut here
}

//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index,
                TCHAR* cmdName,
                PFUNCPLUGINCMD pFunc,
                ShortcutKey* sk,
                bool check0nInit) {
  if (index >= nbFunc)
    return false;

  if (!pFunc)
    return false;

  lstrcpy(funcItem[index]._itemName, cmdName);
  funcItem[index]._pFunc = pFunc;
  funcItem[index]._init2Check = check0nInit;
  funcItem[index]._pShKey = sk;

  return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void about() {
  std::wstringstream info;
  info << L"Notepad++ SaveAsAdmin plugin" << std::endl;
  info << L"Version: " << g_version << std::endl;
  info << L"Author: Khnykin Evgeniy";

  ::MessageBox(NULL, info.str().c_str(), TEXT("About"), MB_OK);
}
