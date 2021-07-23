#pragma once
#include "ConsoleCompile.h"


namespace Filetypes
{
	std::wstring getFileType(LPWSTR lpwstrFilePath);
	std::wstring getFileTypeName(std::wstring wstrFiletype);
};
