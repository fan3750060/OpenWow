#include "stdafx.h"

// General
#include "DebugOutput_Log.h"

#define LOGFILENAME "log.txt"

DebugOutput_Log::DebugOutput_Log()
{
	string fileName = string(LOGFILENAME);

	if (fileName.empty())
	{
		Log::Error("LogFile[%s]: No such file or directory!", fileName.c_str());
		fail1();
	}

	logStream.open(fileName.c_str(), ios::out);
	if (!logStream.is_open())
	{
		Log::Error("LogFile[%s]: Can not open file!", fileName.c_str());
		logStream.clear();
		fail1();
	}
}

DebugOutput_Log::~DebugOutput_Log()
{
	logStream.flush();
	logStream.close();
	logStream.clear();
}

//

void DebugOutput_Log::Print(string _messageFmt, DebugOutput::DebugMessageType _type)
{
	logStream << _messageFmt << endl;
	logStream.flush();
}
