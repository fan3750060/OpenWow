#pragma once

class DebugOutput_Log : public DebugOutput
{
public:
	DebugOutput_Log();
	~DebugOutput_Log();

protected:
	void Print(string _messageFmt, DebugOutput::DebugMessageType _type);

private:
	ofstream logStream;
};
