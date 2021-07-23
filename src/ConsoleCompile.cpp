#include "ConsoleCompile.h"

void ConsoleCompile::CreateConsole()
{
	/*FROM https://justcheckingonall.wordpress.com/2008/08/29/console-window-win32-app/ */
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;
	/*FROM https://justcheckingonall.wordpress.com/2008/08/29/console-window-win32-app/ */
}

void ConsoleCompile::Compile()
{
	wchar_t* filepath;
	filepath = (wchar_t*)VirtualAlloc(NULL, MEM_4MB_PAGES, MEM_COMMIT, PAGE_READWRITE);
	GetCurrFilePath(filepath);
	if (filepath != NULL)
	{
		std::wstring wstrFiletype = Filetypes::getFileType(filepath);

		if (wstrFiletype == L"cpp")
		{
			CompileCpp();
			VirtualFree(filepath, 0, MEM_RELEASE);
		}
		else if (wstrFiletype == L"py")
		{
			CompilePython();
			VirtualFree(filepath, 0, MEM_RELEASE);
		}

	}
}

void ConsoleCompile::CompileCpp()
{
	CreateConsole();

	wchar_t* filepath;
	filepath = (wchar_t*)VirtualAlloc(NULL, MEM_4MB_PAGES, MEM_COMMIT, PAGE_READWRITE);
	GetCurrFilePath(filepath);
	if (filepath != NULL)
	{
		std::wstring wstrFilepath = filepath;
		std::wstring out;
		system("CLS");
		out = L"g++ -o out " + wstrFilepath;
		_wsystem(out.c_str());
		system("out");
		VirtualFree(filepath, 0, MEM_RELEASE);
	}
}

void ConsoleCompile::CompilePython()
{
	CreateConsole();

	wchar_t* filepath;
	filepath = (wchar_t*)VirtualAlloc(NULL, MEM_4MB_PAGES, MEM_COMMIT, PAGE_READWRITE);
	GetCurrFilePath(filepath);
	if (filepath != NULL)
	{
		std::wstring wstrFilepath = filepath;
		std::wstring out;
		system("CLS");
		out = L"python " + wstrFilepath;
		_wsystem(out.c_str());
		VirtualFree(filepath, 0, MEM_RELEASE);
	}
}