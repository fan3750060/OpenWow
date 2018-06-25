#pragma once

#include "DebugOutput.h"

class DebugOutput_Log : public CDebugOutput
{
public:
	DebugOutput_Log();
	~DebugOutput_Log();

protected:
	void Print(string _messageFmt, CDebugOutput::DebugMessageType _type);

private:
	ofstream m_LogStream;
};
