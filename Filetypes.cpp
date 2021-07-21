#include "Filetypes.h"

std::wstring Filetypes::getFileType(LPWSTR lpwstrFilePath)
{
	std::wstring strFilepath = lpwstrFilePath;
	std::size_t foundFiletype = strFilepath.find_last_of(L".");
	std::wstring wstrFiletype = strFilepath.substr(foundFiletype + 1);

	return wstrFiletype;
}

std::wstring Filetypes::getFileTypeName(std::wstring wstrFiletype)
{
	if (wstrFiletype == L"txt")
		return L"Text file (.txt)";
	else if (wstrFiletype == L"html")
		return L"Hypertext Markup Language (.html)";
	else if (wstrFiletype == L"css")
		return L"Cascading Style Sheets (.css)";
	else if (wstrFiletype == L"js")
		return L"Javascript (.js)";
	else if (wstrFiletype == L"cpp")
		return L"C++ source file";
	else
		return L"Unknown";
}