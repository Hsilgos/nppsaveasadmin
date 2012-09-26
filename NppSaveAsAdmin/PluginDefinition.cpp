//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "menuCmdID.h"

#include "FileOperations.h"

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

std::wstring gVersion;

//
// Initialize your plugin data here
// It will be called while plugin loading   
static std::wstring getPath(HANDLE hModule)
{
	TCHAR moduleFile [2048] = { 0 };
	const int tResultSize = GetModuleFileName ((HMODULE)hModule, moduleFile, 2048);
	return std::wstring(moduleFile, tResultSize);
}

static std::wstring getExeVersion (const std::wstring& exeFileName, const std::wstring& fieldName)
{
	DWORD pointlessWin32Variable = 0;
	DWORD size = GetFileVersionInfoSizeW (exeFileName.c_str(), &pointlessWin32Variable);

	if (size > 0)
	{
		std::vector<char> exeInfo;
		exeInfo.resize(size);

		if (GetFileVersionInfoW (exeFileName.c_str(), 0, size, &exeInfo[0]))
		{
			wchar_t* result = 0;
			unsigned int resultLen = 0;

			// try the 1200 codepage (Unicode)
			std::wstring queryStr (L"\\StringFileInfo\\041904B0\\" + fieldName);

			if ( !VerQueryValueW (&exeInfo[0], queryStr.c_str(), (void**) &result, &resultLen) )
			{
				// try the 1252 codepage (Windows Multilingual)
				queryStr = L"\\StringFileInfo\\041904E4\\" + fieldName;
				VerQueryValueW (&exeInfo[0], queryStr.c_str(), (void**) &result, &resultLen);
			}

			if( result && resultLen > 0 )
				return std::wstring (result, resultLen - 1);
		}
	}

	return std::wstring();
}

void pluginInit(HANDLE hModule)
{
	gVersion = getExeVersion(getPath(hModule), L"FileVersion");
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

void unDoInjection();
//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{
    setCommand(0, TEXT("About"), about, NULL, false);

	if( isDebugging() )
	{
		setCommand(1, TEXT("Hook"), doInjection, NULL, false);
		setCommand(2, TEXT("Unhook"), unDoInjection, NULL, false);
	}
    //setCommand(1, TEXT("Hello (with dialog)"), helloDlg, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
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



void about()
{
	std::wstringstream tInfo;
	tInfo << L"Notepad++ SaveAsAdmin plugin" << std::endl;
	tInfo << L"Version: "<< gVersion << std::endl;
	tInfo << L"Author: Khnykin Evgeniy";

	::MessageBox(NULL, tInfo.str().c_str(), TEXT("About"), MB_OK);
}
