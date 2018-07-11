#include "stdafx.h"

// Include
#include "DebugOutput.h"

// General
#include "Log.h"

// Additional
#include "BaseManager.h"
#include "DebugOutputConsole.h"
#include "DebugOutputLog.h"

CLog::CLog()
{
	InitializeCriticalSection(&debugCS);

	AddManager<ILog>(this);

	m_DebugOutput_ConsoleWindows = new DebugOutput_ConsoleWindows;
	AddDebugOutput(m_DebugOutput_ConsoleWindows);
}

CLog::~CLog()
{
	delete m_DebugOutput_ConsoleWindows;

	DelManager<ILog>();

	DeleteCriticalSection(&debugCS);

	OutputDebugString(L"Log destroyed.\n");
}

//--

void CLog::Info(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_INFO, args);
	va_end(args);
}

void CLog::Print(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_PRINT, args);
	va_end(args);
}

void CLog::Green(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_GREEN, args);
	va_end(args);
}

void CLog::Warn(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_WARNING, args);
	va_end(args);
}

void CLog::Error(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_ERROR, args);
	va_end(args);
}

void CLog::Fatal(const char* _message, ...)
{
	FatalMessageBox(_message, "Fatal");
}

//--

bool CLog::AddDebugOutput(IDebugOutput* _debugOutput)
{
	assert1(_debugOutput != nullptr);

	if (find(m_DebugOutputs.begin(), m_DebugOutputs.end(), _debugOutput) != m_DebugOutputs.end())
	{
		return false;
	}

	m_DebugOutputs.push_back(_debugOutput);

	return true;
}

bool CLog::DeleteDebugOutput(IDebugOutput* _debugOutput)
{
	assert1(_debugOutput != nullptr);

	auto _debugOutputsIt = find(m_DebugOutputs.begin(), m_DebugOutputs.end(), _debugOutput);

	// Not exists
	if (_debugOutputsIt == m_DebugOutputs.end())
	{
		return false;
	}

	m_DebugOutputs.erase(_debugOutputsIt);

	return true;
}

void CLog::PushMessageToAllDebugOutputs(const char* _message, IDebugOutput::DebugMessageType _type, va_list& _vaList)
{
	EnterCriticalSection(&debugCS); // THREAD

	for (auto it : m_DebugOutputs)
	{
		it->PushMessage(_type, _message, _vaList);
	}

	LeaveCriticalSection(&debugCS); // THREAD
}

//---------------------------------------------
//-- Static Log
//---------------------------------------------

inline CLog* getLog()
{
	return dynamic_cast<CLog*>(GetManager<ILog>());
}

void Log::Info(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	getLog()->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_INFO, args);
	va_end(args);
}

void Log::Print(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	getLog()->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_PRINT, args);
	va_end(args);
}

void Log::Green(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	getLog()->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_GREEN, args);
	va_end(args);
}

void Log::Warn(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	getLog()->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_WARNING, args);
	va_end(args);
}

void Log::Error(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	getLog()->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_ERROR, args);
	va_end(args);
}

void Log::Fatal(const char* _message, ...)
{
	FatalMessageBox(_message, "Fatal");
}