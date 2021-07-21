#include "Application.h"

static HWND hEdit;
static HWND hStatus;
static HWND hDlgGoToLine;
static HWND hHScrollBar;
static HWND hVScrollBar;
static HWND hTabControl;

static INITCOMMONCONTROLSEX icc;

static std::vector<std::wstring> tabFilePaths;


/* FILES */
static OPENFILENAME ofn = {};
static bool bIsFileCreated;
static bool bTwoBytesPerChar;
/* FILES */

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

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/* This is used for the mouse wheel message. V = Vertical, H = Horizontal */
	static short zDeltaV;
	static short zDeltaH;

	switch (uMsg)
	{
		/* If the next message in the message queue is "WM_CLOSE" or "WM_DESTROY" it calls the
		PostQuitMessage function that adds a NULL message to the message queue. It simply returns 0.
		In the while loop at the bottom of this application a NULL message causes the application to quit.
		*/
	case WM_CREATE:
		break;

	case WM_PAINT:
		break;
	
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE:
			int nIdCurrSel;
			nIdCurrSel = SendMessage(hTabControl, TCM_GETCURSEL, NULL, NULL);
			OpenFile((LPWSTR)tabFilePaths[nIdCurrSel].c_str());
			break;
		}
		break;

	case WM_HOTKEY:
		switch (LOWORD((wParam)))
		{
		case HTKEY_CTRL_S:
			if (bIsFileCreated)
				if(hWnd == GetFocus() || hEdit == GetFocus())
					SaveOpenedFile(hWnd);
			if(!bIsFileCreated)
				if (hWnd == GetFocus() || hEdit == GetFocus())
					SendMessage(hWnd, WM_COMMAND, ID_SAVE_AS, NULL);
			break;
		case HTKEY_CTRL_G:
			if (!IsWindow(hDlgGoToLine))
				hDlgGoToLine = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DlgGoToLineProc);
			if (hDlgGoToLine != NULL)
			{
				ShowWindow(hDlgGoToLine, SW_SHOW);
				SetFocus(GetDlgItem(hDlgGoToLine, IDC_EDIT1));
			}
			break;
		case HTKEY_CTRL_ARR_UP:
			if (hEdit == GetFocus()) SendMessage(hEdit, EM_SCROLL, SB_LINEUP, NULL);
			break;
		case HTKEY_CTRL_ARR_DOWN:
			if (hEdit == GetFocus()) SendMessage(hEdit, EM_SCROLL, SB_LINEDOWN, NULL);
			break;
		case HTKEY_CTRL_SHFT_ARR_UP:
			if (hEdit == GetFocus()) SendMessage(hEdit, EM_SCROLL, SB_PAGEUP, NULL);
			break;
		case HTKEY_CTRL_SHFT_ARR_DOWN:
			if (hEdit == GetFocus()) SendMessage(hEdit, EM_SCROLL, SB_PAGEDOWN, NULL);
			break;
		case HTKEY_CTRL_ALT_S:
			if (hWnd == GetFocus() || hEdit == GetFocus())
				SendMessage(hWnd, WM_COMMAND, ID_SAVE_AS, NULL);
			break;
		}
		break;

	case WM_MOUSEWHEEL:
		zDeltaV = GET_WHEEL_DELTA_WPARAM(wParam);
		if (hEdit == GetFocus()) if (zDeltaV / 120 == 1) SendMessage(hEdit, EM_SCROLL, SB_LINEUP, NULL);
		if (hEdit == GetFocus()) if (zDeltaV / 120 == -1) SendMessage(hEdit, EM_SCROLL, SB_LINEDOWN, NULL);
		break;

	case WM_MOUSEHWHEEL:
		zDeltaH = GET_WHEEL_DELTA_WPARAM(wParam);
		if (hEdit == GetFocus()) if (zDeltaH / 120 == 1) SendMessage(hEdit, WM_HSCROLL, SB_LINERIGHT, NULL);
		if (hEdit == GetFocus()) if (zDeltaH / 120 == -1) SendMessage(hEdit, WM_HSCROLL, SB_LINELEFT, NULL);
		break;

	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(NULL);
		return 0;
	case WM_SETFOCUS:
		SetFocus(hWnd);
		return 0;

		/* When resizing the client window the Edit Text is also resized, so that it matches the client window.*/
	case WM_SIZE:
		MoveWindow(hEdit,
			200, 0 + 25,							//  starting x- and y-coordinates : y is reduced by size of TabControl
			LOWORD(lParam) - 200 - 15,				//  width of client area - some space - width of vertical scroll bar
			HIWORD(lParam) - 23 - 15 - 25,			//  height of client area - height of status bar - height of horizontal scroll bar - height of TabControl
			TRUE);									//  repaint window

		MoveWindow(hStatus,
			0, 0,
			LOWORD(lParam),
			HIWORD(lParam),
			TRUE);

		MoveWindow(hTabControl,
			0, 0,
			LOWORD(lParam),
			25,
			TRUE);

		RECT rect;
		GetClientRect(hWnd, &rect);
		int nVScrollBarSize;
		nVScrollBarSize = 15;
		MoveWindow(hVScrollBar,
			rect.right - nVScrollBarSize, 25,
			nVScrollBarSize,
			rect.bottom - nVScrollBarSize - 8 - 25,
			TRUE);

		int nHScrollBarSize;
		nHScrollBarSize = 15;
		MoveWindow(hHScrollBar,
			200, rect.bottom - nHScrollBarSize - 23,
			rect.right - nHScrollBarSize - 200,
			nHScrollBarSize,
			TRUE);

		return 0;

	case WM_DROPFILES:
		OpenFileDragAndDrop((HDROP)wParam);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_NEW:
			SetWindowText(hEdit, L"\r");
			SendMessage(hWnd, WM_COMMAND, ID_SAVE_AS, NULL);
			break;
		case ID_OPEN:
			OpenFileWithDialog(hWnd, ofn);
			break;
		case ID_SAVE_AS:
			SaveFileWithDialog(hWnd, ofn);
			break;
		case ID_SAVE:
			if (bIsFileCreated)
				SaveOpenedFile(hWnd);
			else
				SendMessage(hWnd, WM_COMMAND, ID_SAVE_AS, NULL);
			break;
		case ID_CLOSE:
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		}

		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
			UpdateStatusBar();
			//PaintTxt(L"Hello");
			break;
		}
		break;

	case WM_LBUTTONUP:
		UpdateStatusBar();
		return 0;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_RIGHT:
		case VK_LEFT:
		case VK_DOWN:
		case VK_UP:
			UpdateStatusBar();
			break;
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RIGHT:
		case VK_LEFT:
		case VK_DOWN:
		case VK_UP:
			UpdateStatusBar();
			break;
		}
		break;

		// Handle messages from the message queue. Messages are user or os related events such as a Mouse Click, Mouse Press, Keyboard Press etc.
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)	// hPrevInstance is always 0.
{
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_BAR_CLASSES | ICC_ANIMATE_CLASS;

	InitCommonControlsEx(&icc);

	HWND hWnd;
	WNDCLASS wcl;
	MSG msg;
	LPCWSTR WND_CLASS_NAME = L"Editor++";
	LPCWSTR WND_WINDOW_NAME = L"Editor++";

	/* Menu creation */
	HMENU hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
	AppendMenu(hFileMenu, MF_STRING, ID_NEW, L"&New");
	AppendMenu(hFileMenu, MF_STRING, ID_OPEN, L"&Open");
	AppendMenu(hFileMenu, MF_STRING, ID_SAVE_AS, L"&Save As...\tCtrl+Alt+S");
	AppendMenu(hFileMenu, MF_STRING, ID_SAVE, L"&Save\tCtrl+S");
	AppendMenu(hFileMenu, MF_STRING, ID_CLOSE, L"&Close\tAlt+F4");
	/* Menu creation */

	wcl.style = CS_HREDRAW | CS_VREDRAW; // Wenn sich das Fenster in der Größe ändert, wird das gesamte Fenster neu gezeichnet.
	wcl.lpfnWndProc = WindowProc;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hInstance = hInstance;
	wcl.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wcl.lpszMenuName = NULL;
	wcl.lpszClassName = WND_CLASS_NAME;

	RegisterClass(&wcl);

	hWnd = CreateWindowEx(WS_EX_ACCEPTFILES, WND_CLASS_NAME, WND_WINDOW_NAME, WS_OVERLAPPEDWINDOW | ES_MULTILINE, CW_USEDEFAULT, CW_USEDEFAULT, 1300, 800, NULL, NULL, hInstance, NULL);

	/*=== Rich edit ===*/
	LoadLibrary(TEXT("Msftedit.dll"));
	hEdit = CreateWindowEx(0, MSFTEDIT_CLASS, L"Hello World!", WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, NULL, hInstance, NULL);
	HFONT hFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, UNICODE, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Consolas"));
	SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hEdit, EM_SETMARGINS, EC_LEFTMARGIN, 70);
	SendMessage(hEdit, EM_SETEVENTMASK, NULL, (LPARAM)ENM_CHANGE);
	/*=== Rich edit ===*/

	/*=== Status bar ===*/
	hStatus = CreateWindowEx(0, STATUSCLASSNAME, L"Filetype waiting...", SBARS_TOOLTIPS | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, NULL, hInstance, NULL);
	int nPartsLocations[] = { 500, 570, 640, -1 };
	int nParts = 4;
	SendMessage(hStatus, SB_SETPARTS, (WPARAM)nParts, (LPARAM)nPartsLocations);
	SendMessage(hStatus, SB_SETTEXT, SendMessage(hStatus, SB_GETPARTS, NULL, NULL) - 1, (LPARAM)L"File to load...");
	UpdateStatusBar();
	/*=== Status bar ===*/

	/*=== Scroll bars ===*/
	// TODO: Scroll bar functionality
	hVScrollBar = CreateWindowEx(0, L"SCROLLBAR", NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, 0, 0, 0, 0, hWnd, NULL, hInstance, NULL);
	hHScrollBar = CreateWindowEx(0, L"SCROLLBAR", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 0, 0, 0, 0, hWnd, NULL, hInstance, NULL);
	/*=== Scroll bars ===*/

	/*=== Tab Control (Holding all tabs) ===*/
	hTabControl = CreateWindowEx(0, WC_TABCONTROL, NULL, WS_CHILD | WS_VISIBLE | TCS_BUTTONS | TCS_FOCUSONBUTTONDOWN | TCS_FLATBUTTONS, 0, 0, 0, 0, hWnd, NULL, hInstance, NULL);
	/*=== Tab Control (Holding all tabs) ===*/

	SetMenu(hWnd, hMenu);


	if (hWnd == NULL)
	{
		return 0;
	}

	InitializeHotKeys();
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (1)	// TODO: Better implementation of while condition.
	{
		BOOL res = GetMessage(&msg, NULL, NULL, NULL);
		if (res > 0) // If there is a message / an event
		{
			TranslateMessage(&msg);
			/* This if statement is used to listen for key-down events in every window not only the client window */
			if (msg.message == WM_KEYDOWN)
			{
				SendMessage(hWnd, WM_KEYDOWN, msg.wParam, msg.lParam);
				if (GetDlgItem(hDlgGoToLine, IDC_EDIT1) == GetFocus()) SendMessage(hDlgGoToLine, WM_KEYDOWN, msg.wParam, msg.lParam);
			}

			if (msg.message == WM_KEYUP)
				SendMessage(hWnd, WM_KEYUP, msg.wParam, msg.lParam);

			if (msg.message == WM_HOTKEY)
				SendMessage(hWnd, WM_HOTKEY, msg.wParam, msg.lParam);

			if (msg.message == WM_MOUSEWHEEL)
				SendMessage(hWnd, WM_MOUSEWHEEL, msg.wParam, msg.lParam);

			if (msg.message == WM_MOUSEHWHEEL)
				SendMessage(hWnd, WM_MOUSEHWHEEL, msg.wParam, msg.lParam);

			if (msg.message == WM_LBUTTONUP)
				SendMessage(hWnd, WM_LBUTTONUP, msg.wParam, msg.lParam);

			DispatchMessage(&msg); // Calls WindowProc function indirectly, once for each message.
		}
		else
		{
			return res;
		}
	}

}

BOOL CALLBACK DlgGoToLineProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	int nLine;
	int nFirstCharacterAtLine;
	int nLastCharacterAtLine;
	BOOL bError;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_RETURN:
			SendMessage(hDlg, WM_COMMAND, IDOK, NULL);
			break;
		case VK_ESCAPE:
			SendMessage(hDlg, WM_COMMAND, IDCANCEL, NULL);
			break;
		}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			nLine = GetDlgItemInt(hDlg, IDC_EDIT1, &bError, NULL) - 1;
			if (bError > 0)	// If this is true there is no error
			{
				SendMessage(hDlg, WM_COMMAND, IDCANCEL, NULL);
				SetFocus(hEdit);
				nFirstCharacterAtLine = SendMessage(hEdit, EM_LINEINDEX, nLine, NULL);
				nLastCharacterAtLine = SendMessage(hEdit, EM_LINEINDEX, nLine + 1, NULL) - 1;
				if (nFirstCharacterAtLine != -1)
					SendMessage(hEdit, EM_SETSEL, nFirstCharacterAtLine, nLastCharacterAtLine);
				UpdateStatusBar();
			}
			break;
		case IDCANCEL:
			DestroyWindow(hDlg);
			return TRUE;
		}
	default:
		return FALSE;

	}
	return TRUE;
}

void SaveOpenedFile(HWND hWnd)
{
	int nPathLength = LOWORD(SendMessage(hStatus, SB_GETTEXTLENGTH, SendMessage(hStatus, SB_GETPARTS, NULL, NULL) - 1, NULL));
	if (nPathLength > 0)
	{
		wchar_t* path;
		path = (wchar_t*)VirtualAlloc(NULL, MEM_4MB_PAGES, MEM_COMMIT, PAGE_READWRITE);

		if (path != NULL)
		{
			if (SendMessage(hStatus, SB_GETTEXT, SendMessage(hStatus, SB_GETPARTS, NULL, NULL) - 1, (LPARAM)path))
			{
				SaveFile(path);
				VirtualFree(path, 0, MEM_RELEASE);
			}
		}
	}
}

void SaveFileWithDialog(HWND hWnd, OPENFILENAME ofn)
{
	char filepath[MAX_FILE_PATH];
	filepath[0] = 0;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = (LPWSTR)filepath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_FILE_PATH;
	ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrTitle = L"Save file";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	ofn.lpstrDefExt = L".txt";
	if (GetSaveFileName(&ofn))
		SaveFile((LPWSTR)filepath);
}

void SaveFile(LPWSTR lpwstrFilePath)
{
	SendMessage(hStatus, SB_SETTEXT, SendMessage(hStatus, SB_GETPARTS, NULL, NULL) - 1, (LPARAM)lpwstrFilePath);

	HANDLE hFile;
	hFile = CreateFile((LPCWSTR)lpwstrFilePath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		int len = GetWindowTextLength(hEdit);
		if (len > 0)
		{
			std::wstring strFilepath = lpwstrFilePath;
			std::size_t foundFilename = strFilepath.find_last_of(L"\\");
			std::wstring wstrFilename = strFilepath.substr(foundFilename + 1);

			UpdateFileTypeNameInStatusBar(lpwstrFilePath);

			if (bTwoBytesPerChar)
			{
				wchar_t* Wbuf;
				Wbuf = (wchar_t*)VirtualAlloc(NULL, MEM_4MB_PAGES, MEM_COMMIT, PAGE_READWRITE);
				if (Wbuf != NULL)
					if (GetWindowTextW(hEdit, Wbuf, len + 1))
					{
						DWORD dwWritten;
						WriteFile(hFile, Wbuf, len * 2, &dwWritten, NULL);
						VirtualFree(Wbuf, 0, MEM_RELEASE);
						CloseHandle(hFile);
						bIsFileCreated = true;
						AddTab(hTabControl, (LPWSTR)wstrFilename.c_str(), lpwstrFilePath);

					}

			}
			else if (!bTwoBytesPerChar)
			{
				char* Abuf;
				Abuf = (char*)VirtualAlloc(NULL, MEM_4MB_PAGES, MEM_COMMIT, PAGE_READWRITE);
				if (Abuf != NULL)
					if (GetWindowTextA(hEdit, (LPSTR)Abuf, len + 1))
					{
						DWORD dwWritten;
						WriteFile(hFile, Abuf, len, &dwWritten, NULL);
						VirtualFree(Abuf, 0, MEM_RELEASE);
						CloseHandle(hFile);
						bIsFileCreated = true;
						AddTab(hTabControl, (LPWSTR)wstrFilename.c_str(), lpwstrFilePath);
					}
			}

		}
	}
}

void OpenFileWithDialog(HWND hWnd, OPENFILENAME ofn)
{
	char filepath[MAX_FILE_PATH];
	filepath[0] = 0;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = (LPWSTR)filepath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_FILE_PATH;
	ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrTitle = L"Open file";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER;
	ofn.lpstrDefExt = L".txt";

	if (GetOpenFileName(&ofn))
		OpenFile((LPWSTR)filepath);
}

void OpenFileDragAndDrop(HDROP hDrop)
{
	wchar_t filepath[MAX_FILE_PATH];
	DragQueryFile(hDrop, NULL, filepath, MAX_FILE_PATH);
	OpenFile(filepath);
	DragFinish(hDrop);
}

void OpenFile(LPWSTR lpwstrFilePath)
{
	HANDLE hFile;
	hFile = CreateFile((LPCWSTR)lpwstrFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFilesize = GetFileSize(hFile, NULL);
		if (dwFilesize != 0xFFFFFFFF)
		{
			char* buf;
			buf = (char*)VirtualAlloc(NULL, MEM_4MB_PAGES, MEM_COMMIT, PAGE_READWRITE);

			if (buf != NULL)
			{
				DWORD dwRead;
				if (ReadFile(hFile, buf, dwFilesize, &dwRead, NULL))
				{
					std::wstring strFilepath = lpwstrFilePath;
					std::size_t foundFilename = strFilepath.find_last_of(L"\\");
					std::wstring wstrFilename = strFilepath.substr(foundFilename + 1);

					UpdateFileTypeNameInStatusBar(lpwstrFilePath);

					std::string s = buf;
					int nAmountOfCharactersInFile = s.length();
					buf[dwFilesize] = 0;
					if (nAmountOfCharactersInFile * dwFilesize == dwFilesize && dwFilesize != NULL)
					{
						bTwoBytesPerChar = true;
						SetWindowTextW(hEdit, (LPCWSTR)buf);
						bIsFileCreated = true;
						AddTab(hTabControl, (LPWSTR)wstrFilename.c_str(), lpwstrFilePath);
					}
					else
					{
						bTwoBytesPerChar = false;
						SetWindowTextA(hEdit, buf);
						bIsFileCreated = true;
						AddTab(hTabControl, (LPWSTR)wstrFilename.c_str(), lpwstrFilePath);
					}
					SendMessage(hStatus, SB_SETTEXT, SendMessage(hStatus, SB_GETPARTS, NULL, NULL)-1, (LPARAM)lpwstrFilePath);
					VirtualFree(buf, 0, MEM_RELEASE);
					CloseHandle(hFile);
					UpdateStatusBar();
				}
			}
		}
	}
}

void UpdateStatusBar()
{

	/* Show Number of lines */
	int nLineCount = SendMessage(hEdit, EM_GETLINECOUNT, NULL, NULL);
	SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)std::to_wstring(nLineCount).c_str());

	/* Show Current line */
	int nCurrLine = SendMessage(hEdit, EM_LINEFROMCHAR, -1, NULL);
	std::wstring wstrCurrLine = std::to_wstring(nCurrLine + 1).c_str();
	std::wstring wstrOutputCurrLine = L"Line: " + wstrCurrLine;
	SendMessage(hStatus, SB_SETTEXT, 2, (LPARAM)wstrOutputCurrLine.c_str());
}

void UpdateFileTypeNameInStatusBar(LPWSTR lpwstrFilePath)
{
	std::wstring wstrFileType = Filetypes::getFileType(lpwstrFilePath);
	std::wstring wstrFileTypeName = Filetypes::getFileTypeName(wstrFileType);
	SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)wstrFileTypeName.c_str());
}

/* Adds a tab at the end of the Tab Control */
void AddTab(HWND hTabControl, LPWSTR lpwstrTabName, LPWSTR lpwstrFilePath)
{
	/* Checks if the given file path is already present in the tabFilePaths vector and hence in the Tab Control.
	   The statement is true if no element is found i.e. the file path does not exist. */
	if (std::find(tabFilePaths.begin(), tabFilePaths.end(), lpwstrFilePath) == tabFilePaths.end())
	{
		int nId;
		int nTabItemCount;
		TCITEM tcitem;

		tcitem.mask = TCIF_TEXT;
		tcitem.pszText = lpwstrTabName;
		nTabItemCount = SendMessage(hTabControl, TCM_GETITEMCOUNT, NULL, NULL);
		nId = nTabItemCount == 0 ? 0 : nTabItemCount;

		SendMessage(hTabControl, TCM_INSERTITEM, nId, (LPARAM)&tcitem);
	
		SendMessage(hTabControl, TCM_SETCURFOCUS, nId, NULL);
		SendMessage(hTabControl, TCM_SETCURSEL, nId, NULL);

		tabFilePaths.push_back(lpwstrFilePath);
	}
	else
	{
		/* Gets the index of the requested file path and sets the focus for this specific item.
		   The requested file path is already an item of Tab Control. */
		std::vector<std::wstring>::iterator it;
		it = std::find(tabFilePaths.begin(), tabFilePaths.end(), lpwstrFilePath);
		int nDuplicateTabIndex = std::distance(tabFilePaths.begin(), it);
		SendMessage(hTabControl, TCM_SETCURFOCUS, nDuplicateTabIndex, NULL);
		SendMessage(hTabControl, TCM_SETCURSEL, nDuplicateTabIndex, NULL);
	}
}

// TODO:::SYNTAX HIGHLIGHTING
/*
void PaintTxt(LPCWSTR word) 
{
	FINDTEXT ft;
	ft.chrg.cpMin = 0;
	ft.chrg.cpMax = GetWindowTextLength(hEdit);
	ft.lpstrText = (LPCWSTR)word;
	std::wstring s(word);

	int res = SendMessage(hEdit, EM_FINDTEXT, FR_DOWN | FR_WHOLEWORD, (LPARAM)&ft);
	if (res != -1)
	{
		int selWord = SendMessage(hEdit, EM_SETSEL, res, s.length());
		OutputDebugString(ft.lpstrText);
		CHARFORMAT cf;
		cf.cbSize = sizeof(cf);
		cf.dwMask = CFM_COLOR;
		cf.crTextColor = RGB(255, 0, 0);
		cf.dwEffects = 0;
		SendMessage(hEdit, EM_SETCHARFORMAT, SCF_WORD | SCF_SELECTION, (LPARAM)&cf);
		HDC hdc = GetDC(hEdit);
		SetTextColor(hdc, RGB(0, 0, 0));
	}
}
*/
