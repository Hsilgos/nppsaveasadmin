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
#include "plugin/SaveAsAdminVersion.hpp"

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

static std::wstring get_path(HANDLE hModule) {
  TCHAR module_file[2048] = {0};
  const int result_size =
      GetModuleFileName((HMODULE)hModule, module_file, 2048);
  return std::wstring(module_file, result_size);
}

//
// Initialize your plugin data here
// It will be called while plugin loading
void pluginInit(HANDLE hModule) {
}

//
// Here you can do the clean up, save the parameters (if any) for the next
// session
//
void pluginCleanUp() {}

void do_injection();
void un_do_injection();

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit() {
  setCommand(0, TEXT("About"), about, NULL, false);

  if (is_debugging()) {
    setCommand(1, TEXT("Hook"), do_injection, NULL, false);
    setCommand(2, TEXT("Unhook"), un_do_injection, NULL, false);
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
  info << L"Version: " << SAVE_AS_ADMIN_VERSION << std::endl;
  info << L"Author: Khnykin Yauheni";

  ::MessageBox(NULL, info.str().c_str(), TEXT("About"), MB_OK);
}
