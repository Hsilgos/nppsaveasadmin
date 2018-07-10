#include <Windows.h>

#include "CommandManager.hpp"
#include "CommandProcessor.hpp"
#include "Common.hpp"
#include "IWinApiFunctions.hpp"

int WINAPI CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  int arg_count = 0;
  LPWSTR* args = CommandLineToArgvW(GetCommandLineW(), &arg_count);

  if (arg_count < 2) {
    MessageBox(0, TEXT("Do not run this application directly"), TEXT("Error"),
               MB_OK);
    return WrongExecute;
  }
  DefaultWinApiFunctions winapi;
  CommandManager command_manager;
  register_default_commands(command_manager, winapi);
  return process_commands(command_manager, args[1], args[2]);
}
