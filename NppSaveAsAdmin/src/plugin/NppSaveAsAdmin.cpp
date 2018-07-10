#include "../PluginDefinition.h"

#include <set>
#include <string>
#include <vector>

#include "plugin/AdminAccess.hpp"
#include "plugin/SaveAsAdminImpl.hpp"

#include "IWinApiFunctions.hpp"

extern FuncItem funcItem[nbFunc];
extern NppData nppData;

namespace {
std::unique_ptr<SaveAsAdminImpl> m_save_as_admin_impl;
}

//////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain(HANDLE hModule, DWORD reasonForCall, LPVOID lpReserved) {
  switch (reasonForCall) {
    case DLL_PROCESS_ATTACH:
      pluginInit(hModule);
      delete_admin_access();
      // do_injection();
      break;

    case DLL_PROCESS_DETACH:
      commandMenuCleanUp();
      pluginCleanUp();
      delete_admin_access();
      // un_do_injection();
      break;

    case DLL_THREAD_ATTACH:
      break;

    case DLL_THREAD_DETACH:
      break;
  }

  return TRUE;
}

extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData) {
  nppData = notpadPlusData;
  commandMenuInit();
}

extern "C" __declspec(dllexport) const TCHAR* getName() {
  return NPP_PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem* getFuncsArray(int* nbF) {
  *nbF = nbFunc;
  return funcItem;
}

std::wstring current_file_path(SCNotification* notifyCode) {
  int avar = SendMessage(nppData._nppHandle, NPPM_GETFULLPATHFROMBUFFERID,
                         (WPARAM)notifyCode->nmhdr.idFrom, (LPARAM)NULL);

  if (avar == 0)
    return std::wstring();

  std::vector<wchar_t> theFullPath(avar + 1);
  theFullPath[0] = 0;
  avar =
      SendMessage(nppData._nppHandle, NPPM_GETFULLPATHFROMBUFFERID,
                  (WPARAM)notifyCode->nmhdr.idFrom, (LPARAM)theFullPath.data());

  std::wstring tPath = theFullPath.data();

  return tPath;
}

extern "C" __declspec(dllexport) void beNotified(SCNotification* notifyCode) {
  switch (notifyCode->nmhdr.code) {
    case NPPN_FILEBEFORESAVE:
		if (m_save_as_admin_impl) {
			m_save_as_admin_impl->allow_process_file(current_file_path(notifyCode));
		}
      break;
    case NPPN_FILESAVED:
		if (m_save_as_admin_impl) {
			m_save_as_admin_impl->cancel_process_file(current_file_path(notifyCode));
		}
      break;
    default:
      break;
  }
}

// Here you can process the Npp Messages
// I will make the messages accessible little by little, according to the need
// of plugin development. Please let me know if you need to access to some
// messages : http://sourceforge.net/forum/forum.php?forum_id=482781
//
extern "C" __declspec(dllexport) LRESULT
    messageProc(UINT Message, WPARAM wParam, LPARAM lParam) {
  return TRUE;
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode() {
  return TRUE;
}
#endif  // UNICODE