#pragma once
#include "Application.h"

namespace Toolbar
{
	HWND CreateToolBar(HWND hWnd, HINSTANCE hInstance);

	const int nButtonAmount = 5;
	// Defines the toolbar buttons
	static TBBUTTON tbButtons[nButtonAmount] = {
		{ MAKELONG(STD_FILENEW,  0), ID_NEW,  TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"New File"},
		{ MAKELONG(STD_FILEOPEN, 0), ID_OPEN, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"Open"},
		{ MAKELONG(STD_FILESAVE, 0), ID_SAVE, TBSTATE_ENABLED,               BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"Save"},
		{ MAKELONG(STD_FILESAVE, 0), ID_SAVE_AS, TBSTATE_ENABLED,            BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"Save As..."},
		{ MAKELONG(STD_PROPERTIES, 0), ID_COMPILE, TBSTATE_ENABLED,                BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"Run"}
	};
};
