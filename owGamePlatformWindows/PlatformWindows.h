#pragma once

class PlatformWindows
{
public:
	static IOpenGLAdapter* CreateAdapter_GLFW();

	static DebugOutput* CreateDebugOutput_ConsoleWindows();
	static DebugOutput* CreateDebugOutput_Log();
};