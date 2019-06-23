#include "../PluginDefinition.h"

#include <array>
#include <string>

#include "plugin/AdminAccess.hpp"
#include "plugin/SaveAsAdminImpl.hpp"

#include "IWinApiFunctions.hpp"

extern FuncItem funcItem[nbFunc];
extern NppData nppData;

namespace {
std::wstring g_admin_app_path;
std::unique_ptr<AdminAccessRunner> g_admin_access_runner;
std::unique_ptr<SaveAsAdminImpl> g_save_as_admin_impl;
}

void do_injection() {
	if (!g_save_as_admin_impl) {
		g_admin_access_runner = AdminAccessRunner::make_default(g_admin_app_path);
		g_save_as_admin_impl = std::make_unique<SaveAsAdminImpl>(*g_admin_access_runner);
	}
}

void un_do_injection() {
	g_save_as_admin_impl.reset();
}

std::wstring get_module_path(HANDLE hModule) {
	using Buffer = std::array<TCHAR, 2048>;
	Buffer module_file = { { 0 } };
	const int result_size =
		GetModuleFileName(static_cast<HMODULE>(hModule), module_file.data(), static_cast<DWORD>(module_file.size()));
	return std::wstring(module_file.data(), result_size);
}

std::wstring get_admin_app_path(HANDLE hModule) {
	std::wstring module_path = get_module_path(hModule);
	const auto sep_position = module_path.find_last_of(L"\\/");
	if (sep_position != std::wstring::npos) {
		module_path.replace(sep_position + 1, std::wstring::npos, L"NppAdminAccess.exe");
		return module_path;
	}
	return std::wstring();
}

//////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain(HANDLE hModule, DWORD reasonForCall, LPVOID /*lpReserved*/) {
  switch (reasonForCall) {
    case DLL_PROCESS_ATTACH:
	  g_admin_app_path = get_admin_app_path(hModule);
	  if (!g_admin_app_path.empty()) {
		  pluginInit(hModule);
		  do_injection();
	  }
      break;

    case DLL_PROCESS_DETACH:
      commandMenuCleanUp();
      pluginCleanUp();
      un_do_injection();
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

extern "C" __declspec(dllexport) void beNotified(SCNotification* notifyCode) {
  switch (notifyCode->nmhdr.code) {
    case NPPN_FILEBEFORESAVE:
		if (g_save_as_admin_impl) {
			g_save_as_admin_impl->allow_process_file();
		}
      break;
    case NPPN_FILESAVED:
		if (g_save_as_admin_impl) {
			g_save_as_admin_impl->cancel_process_file();
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
    messageProc(UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/) {
  return TRUE;
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode() {
  return TRUE;
}
#endif  // UNICODE
