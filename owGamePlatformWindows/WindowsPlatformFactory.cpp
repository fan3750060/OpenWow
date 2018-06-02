#include "stdafx.h"

// General
#include "WindowsPlatformFactory.h"

// Additiobal
#include "DebugOutputConsole.h"
#include "DebugOutputLog.h"
#include "OpenGLAdapterGLFW.h"

CWindowsPlatformFactory::CWindowsPlatformFactory()
{
	AddManager<IPlatformFactory>(this);
}

CWindowsPlatformFactory::~CWindowsPlatformFactory()
{
	DelManager<IPlatformFactory>();
}

//--

IOpenGLAdapter* CWindowsPlatformFactory::CreateOpenGLAdapter()
{
	return new OpenGLAdapter_GLFW();
}

IDebugOutput* CWindowsPlatformFactory::CreateDebugOutputLog()
{
	return new DebugOutput_Log();
}

IDebugOutput* CWindowsPlatformFactory::CreateDebugOutputConsole()
{
	return new DebugOutput_ConsoleWindows();
}