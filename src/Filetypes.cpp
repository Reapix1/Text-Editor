#include "Filetypes.h"

// Returns types such as .txt or .html
std::wstring Filetypes::getFileType(LPWSTR lpwstrFilePath)
{
	std::wstring strFilepath = lpwstrFilePath;
	std::size_t foundFiletype = strFilepath.find_last_of(L".");
	std::wstring wstrFiletype = strFilepath.substr(foundFiletype + 1);

	return wstrFiletype;
}

// File type logic. Gives different file types a name and enables or disables compiling
std::wstring Filetypes::getFileTypeName(std::wstring wstrFiletype)
{
	HideCompileButton();
	if (wstrFiletype == L"txt")
		return L"Text file (.txt)";
	else if (wstrFiletype == L"html")
		return L"Hypertext Markup Language (.html)";
	else if (wstrFiletype == L"css")
		return L"Cascading Style Sheets (.css)";
	else if (wstrFiletype == L"xml")
		return L"Extensible Markup Language (.xml)";
	else if (wstrFiletype == L"js")
		return L"Javascript source file (.js)";
	else if (wstrFiletype == L"c")
		return L"C source file (.c)";
	else if (wstrFiletype == L"cpp")
	{
		ShowCompileButton();
		return L"C++ source file (.cpp)";
	}
	else if (wstrFiletype == L"cs")
		return L"Visual C# source file (.cs)";
	else if (wstrFiletype == L"py")
		return L"Python source file (.py)";
	else if (wstrFiletype == L"java")
		return L"Java source file (.java)";
	else if (wstrFiletype == L"php")
		return L"PHP source file (.php)";
	else
		return wstrFiletype;
}