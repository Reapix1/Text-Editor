#pragma once
#pragma warning(disable:4996)

#pragma comment(lib, "comctl32.lib")

#ifndef UNICODE
#define UNICODE
#endif 

#define ID_NEW 101
#define ID_OPEN 102
#define ID_SAVE 103
#define ID_CLOSE 104


#define MAX_FILE_PATH 2048

#include <iostream>
#include <string>
#include <windows.h>
#include <CommCtrl.h>
#include <Richedit.h>

#include "res/resource1.h"
#include "HotKeys.h"

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
