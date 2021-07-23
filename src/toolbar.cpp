#include "Toolbar.h"

HWND Toolbar::CreateToolBar(HWND hWnd, HINSTANCE hInstance)
{
	HWND hToolBar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS, 0, 0, 0, 0, hWnd, NULL, hInstance, NULL);

	// Loads the standard system button images
	SendMessage(hToolBar, TB_LOADIMAGES, (WPARAM)IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);
	SendMessage(hToolBar, TB_SETMAXTEXTROWS, 0, 0); // Enables tooltips to display. It sets the number of rows to 0, hence there is no space for text to display.

	// Adds the defined buttons to the toolbar
	SendMessage(hToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hToolBar, TB_ADDBUTTONS, (WPARAM)Toolbar::nButtonAmount, (LPARAM)&Toolbar::tbButtons);

	// Resize the toolbar, and then show it
	SendMessage(hToolBar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hToolBar, SW_SHOW);

	return hToolBar;
}