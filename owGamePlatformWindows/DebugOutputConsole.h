#pragma once

class DebugOutput_ConsoleWindows : public CDebugOutput
{
public:
	DebugOutput_ConsoleWindows();
	~DebugOutput_ConsoleWindows();

protected:
	// CDebugOutput
	void Print(string _messageFmt, CDebugOutput::DebugMessageType _type);

private:
	enum ConsoleWindowsColor
	{
		GRAY = 8,
		BLUE = 9,
		GREEN = 10,
		LIGHTBLUE = 11,
		RED = 12,
		PINK = 13,
		YELLOW = 14,
		WHITE = 15
	};

	HANDLE m_ConsoleHandle;
	uint16 m_DefaultConsoleColor;
};
