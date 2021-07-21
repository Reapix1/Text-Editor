#pragma once
#include <Windows.h>
#include <iostream>

class Filetypes
{
public:
	static std::wstring getFileType(LPWSTR lpwstrFilePath);
	static std::wstring getFileTypeName(std::wstring wstrFiletype);
};