#pragma once

class CLog : public ILog
{
	friend class Log;
public:
    CLog();
	~CLog();

    void Info(const char* _message, ...);
    void Print(const char* _message, ...);
    void Green(const char* _message, ...);
    void Warn(const char* _message, ...);
    void Error(const char* _message, ...);
    void Fatal(const char* _message, ...);

	// ILog
	bool AddDebugOutput(IDebugOutput* _debugOutput) override;
	bool DeleteDebugOutput(IDebugOutput* _debugOutput) override;

private:
    void PushMessageToAllDebugOutputs(const char* _message, IDebugOutput::DebugMessageType _type, va_list& _vaList);

private:
	IDebugOutput* m_DebugOutput_ConsoleWindows;

    vector<IDebugOutput*> m_DebugOutputs;
    CRITICAL_SECTION debugCS;
};

// Helper class to fast access

class Log
{
public:
	static void Info(const char* _message, ...);
	static void Print(const char* _message, ...);
	static void Green(const char* _message, ...);
	static void Warn(const char* _message, ...);
	static void Error(const char* _message, ...);
	static void Fatal(const char* _message, ...);
};