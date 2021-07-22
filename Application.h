#pragma once
#pragma warning(disable:4996)

#pragma comment(lib, "comctl32.lib")

#ifndef UNICODE
#define UNICODE
#endif 

/* MENU BUTTONS */
#define ID_NEW 101
#define ID_OPEN 102
#define ID_SAVE_AS 103
#define ID_SAVE 104
#define ID_CLOSE 105
#define ID_COMPILE 106


#define MAX_FILE_PATH 2048

#include <iostream>
#include <string>
#include <windows.h>
#include <CommCtrl.h>
#include <Richedit.h>
#include <vector>
#include <algorithm>
#include <fcntl.h>
#include <io.h>

#include "res/resource1.h"
#include "Hotkeys.h"
#include "Filetypes.h"
#include "ConsoleCompile.h"
#include "Toolbar.h"


#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

BOOL CALLBACK DlgGoToLineProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void SaveOpenedFile(HWND hWnd);
void SaveFileWithDialog(HWND hWnd, OPENFILENAME ofn);
void SaveFile(LPWSTR lpwstrFilePath);
void OpenFileWithDialog(HWND hWnd, OPENFILENAME ofn);
void OpenFileDragAndDrop(HDROP hDrop);
void OpenFile(LPWSTR lpwstrFilePath);
void UpdateStatusBar();
void UpdateFileTypeNameInStatusBar(LPWSTR lpwstrFilePath);
void AddTab(HWND hTabControl, LPWSTR lpwstrTabName, LPWSTR lpwstrFilePath);
void GetCurrFilePath(wchar_t* filepath);
void ShowCompileButton();
void HideCompileButton();