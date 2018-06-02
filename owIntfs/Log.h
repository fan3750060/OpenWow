#pragma once

// Forward BEGIN
__interface IManager;
__interface IDebugOutput;
// Forward END

__interface 
	__declspec(uuid("114337A3-0FD0-4AB4-9CBC-40B6FE4E1796"))
	ILog : IManager
{
	/*void Info(const char* _message, ...);
	void Print(const char* _message, ...);
	void Green(const char* _message, ...);
	void Warn(const char* _message, ...);
	void Error(const char* _message, ...);
	void Fatal(const char* _message, ...);*/

	bool AddDebugOutput(IDebugOutput* _debugOutput);
	bool DeleteDebugOutput(IDebugOutput* _debugOutput);
};