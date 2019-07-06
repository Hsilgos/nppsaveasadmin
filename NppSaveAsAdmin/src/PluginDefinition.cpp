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
#include "Resource.h"
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
HINSTANCE module_handle;

//
// Initialize your plugin data here
// It will be called while plugin loading
void pluginInit(HINSTANCE hModule) {
  module_handle = hModule;
}

//
// Here you can do the clean up, save the parameters (if any) for the next
// session
//
void pluginCleanUp() {}

void do_injection();
void un_do_injection();
bool is_admin_app_exists();

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

std::string make_about_text() {
  std::stringstream info;
  info << "Notepad++ SaveAsAdmin plugin" << std::endl;
  info << "Version: " << SAVE_AS_ADMIN_VERSION << std::endl;
  info << "Author: Khnykin Yauheni" << std::endl;

  if (is_admin_app_exists()) {
    info << std::endl
         << "Allows to save file as administrator with UAC prompt."
         << std::endl;
    info << "Just save file as you usually do with menu, hotkey, etc."
         << std::endl;
  } else {
    info << std::endl
         << "Something went wrong! Importand files are missed, please try to "
            "reinstall."
         << std::endl;
  }

  info << std::endl
       << "Plugin sources: https://github.com/Hsilgos/nppsaveasadmin"
       << std::endl;
  info << "Issue reports: https://github.com/Hsilgos/nppsaveasadmin/issues"
       << std::endl;
  return info.str();
}

constexpr WPARAM SCINTILLA_STYLE_LINK = 2;

struct LinkInfo {
  size_t position;
  std::string link;
};

std::vector<LinkInfo> find_links(
    const std::string& text,
    const std::vector<std::string>& link_prefixes) {
  std::vector<LinkInfo> result;
  for (const std::string& prefix : link_prefixes) {
    auto position = text.find(prefix);
    while (position != std::string::npos) {
      auto end_position = text.find_first_of(" \n", position);
      if (end_position == std::string::npos) {
        end_position = text.size();
      }

      LinkInfo link_info;
      link_info.position = position;
      link_info.link = text.substr(position, end_position - position);

      result.push_back(link_info);

      position = text.find(prefix, end_position);
    }
  }
  return result;
}

void apply_links(HWND hDlg,
                 int scintilla_id,
                 const std::string& text,
                 const std::vector<std::string>& link_prefixes) {
  for (const LinkInfo& link_info : find_links(text, link_prefixes)) {
    SendDlgItemMessage(hDlg, scintilla_id, SCI_STARTSTYLING, link_info.position,
                       0);
    SendDlgItemMessage(hDlg, scintilla_id, SCI_SETSTYLING,
                       link_info.link.size(), SCINTILLA_STYLE_LINK);
  }
}

std::string find_link_at_position(
    const std::string& text,
    std::uint32_t position,
    const std::vector<std::string>& link_prefixes) {
  for (const LinkInfo& link_info : find_links(text, link_prefixes)) {
    if (position >= link_info.position &&
        position < link_info.position + link_info.link.size()) {
      return link_info.link;
    }
  }

  return std::string();
}

const std::vector<std::string> link_prefixes = {"https://", "mailto:"};

void init_scintilla(HWND hDlg) {
  const std::string about_text = make_about_text();
  SendDlgItemMessage(hDlg, IDC_SCINTILLA, SCI_SETTEXT, 0,
                     reinterpret_cast<LPARAM>(about_text.c_str()));
  SendDlgItemMessage(hDlg, IDC_SCINTILLA, SCI_SETREADONLY, TRUE, 0);
  SendDlgItemMessage(hDlg, IDC_SCINTILLA, SCI_SETHSCROLLBAR, FALSE, 0);

  SendDlgItemMessage(hDlg, IDC_SCINTILLA, SCI_SETHOTSPOTACTIVEFORE, TRUE,
                     0xFF0000);
  SendDlgItemMessage(hDlg, IDC_SCINTILLA, SCI_STYLESETHOTSPOT,
                     SCINTILLA_STYLE_LINK, TRUE);

  apply_links(hDlg, IDC_SCINTILLA, about_text, link_prefixes);
}

void center_dialog(HWND hDlg) {
  const HWND hwndOwner = GetParent(hDlg);

  RECT rect_owner;
  GetWindowRect(hwndOwner, &rect_owner);

  RECT rect_dlg;
  GetWindowRect(hDlg, &rect_dlg);

  RECT result_rect;
  CopyRect(&result_rect, &rect_owner);

  OffsetRect(&rect_dlg, -rect_dlg.left, -rect_dlg.top);
  OffsetRect(&result_rect, -result_rect.left, -result_rect.top);
  OffsetRect(&result_rect, -rect_dlg.right, -rect_dlg.bottom);

  SetWindowPos(hDlg, HWND_TOP, rect_owner.left + (result_rect.right / 2),
               rect_owner.top + (result_rect.bottom / 2), 0, 0, SWP_NOSIZE);
}

// Message handler for about box.
INT_PTR CALLBACK about_dlg_proc(HWND hDlg,
                                UINT message,
                                WPARAM wParam,
                                LPARAM lParam) {
  switch (message) {
    case WM_INITDIALOG: {
      center_dialog(hDlg);
      init_scintilla(hDlg);

      return (INT_PTR)TRUE;
    }

    case WM_NOTIFY: {
      if (wParam == IDC_SCINTILLA) {
        const SCNotification* notification =
            reinterpret_cast<SCNotification*>(lParam);
        switch (notification->nmhdr.code) {
          case SCN_HOTSPOTCLICK:
          case SCN_HOTSPOTDOUBLECLICK: {
            const std::string link = find_link_at_position(
                make_about_text(), notification->position, link_prefixes);
            ShellExecuteA(NULL, "open", link.c_str(), NULL, NULL,
                          SW_SHOWNORMAL);
            break;
          }
        }
      }

      break;
    }

    case WM_COMMAND:
      if (LOWORD(wParam) == IDCLOSE || LOWORD(wParam) == IDCANCEL) {
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
      }
      break;
  }
  return (INT_PTR)FALSE;
}

void about() {
  DialogBox(module_handle, MAKEINTRESOURCE(IDD_ABOUT_SAVE_AS_ADMIN),
            nppData._nppHandle, about_dlg_proc);
}
