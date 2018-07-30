#pragma once

#include <owIntfs.h>
#include <owCore.h>

#include "common.h"

#include <comdef.h>
#include <atlbase.h>

#include <d3d11_2.h>
#pragma comment(lib, "d3d11.lib")

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <dxgi1_3.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

// STL
#include <codecvt>
#include <sstream>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <ctime>
#include <algorithm>
#include <atomic>
#include <mutex>
#include <thread>

// Convert a multi-byte character string (UTF-8) to a wide (UTF-16) encoded string.
inline std::wstring ConvertString(cstring string)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(string);
}

// Converts a wide (UTF-16) encoded string into a multi-byte (UTF-8) character string.
inline std::string ConvertString(const std::wstring& wstring)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(wstring);
}

// Gets a string resource from the module's resources.
inline std::string GetStringResource(int ID, cstring type)
{
	HMODULE hModule = GetModuleHandle(nullptr);
	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(ID), type.c_str());
	if (hResource)
	{
		HGLOBAL hResourceData = LoadResource(hModule, hResource);
		DWORD resourceSize = SizeofResource(hModule, hResource);
		if (hResourceData && resourceSize > 0)
		{
			const char* resourceData = static_cast<const char*>(LockResource(hResourceData));
			std::string strData(resourceData, resourceSize);
			return strData;
		}
	}
	// Just return an empty string.
	return std::string();
}