#pragma once

// Forward BEGIN
struct IManager;
struct IDebugOutput;
// Forward END

struct 
	__declspec(novtable, uuid("114337A3-0FD0-4AB4-9CBC-40B6FE4E1796"))
	ILog : IManager
{
	/*void Info(const char* _message, ...);
	void Print(const char* _message, ...);
	void Green(const char* _message, ...);
	void Warn(const char* _message, ...);
	void Error(const char* _message, ...);
	void Fatal(const char* _message, ...);*/

	virtual bool AddDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput) = 0;
	virtual bool DeleteDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput) = 0;
};