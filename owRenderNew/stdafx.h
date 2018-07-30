#pragma once

#include <owIntfs.h>
#include <owCore.h>

#include "common.h"
#include "resource.h"

#include <process.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <comdef.h>
#include <CommCtrl.h> // For windows controls (progress bar)

// Windows Runtime library (needed for Microsoft::WRL::ComPtr<> template class)
#include <wrl.h>

#include <d3d11_2.h> // <d3d11_3.h> is only supported on Windows 10 (but then I'd rather use DX12!?)
#pragma comment(lib, "d3d11.lib")

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <dxgi1_3.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

// STL
#include <locale>
#include <codecvt>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <map>
#include <ctime>
#include <algorithm>

#include <atomic>
#include <mutex>
#include <thread>

// BOOST
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
#include <boost/math/special_functions/round.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/signals2.hpp>
#include <boost/any.hpp>

// Report an error to the Debug output in Visual Studio, display a message box with the error message and throw an exception.
inline void ReportErrorAndThrow(cstring file, int line, cstring function, cstring message)
{
	std::stringstream ss;

	DWORD errorCode = GetLastError();
	LPTSTR errorMessage = nullptr;

	FormatMessage
	(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, 
		errorCode, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		errorMessage, 
		0, 
		nullptr
	);

	if (errorMessage)
	{
		ss << file << "(" << line << "): " << "error " << errorCode << ": " << errorMessage << std::endl;
		LocalFree(errorMessage);
	}
	else
	{
		ss << file << "(" << line << "): " << message << std::endl;
	}

	OutputDebugString(ss.str().c_str());
	MessageBox(nullptr, message.c_str(), function.c_str(), MB_ICONERROR);

	throw new std::exception(message.c_str());
}

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
	HRSRC hResource = FindResourceA(hModule, MAKEINTRESOURCE(ID), type.c_str());
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