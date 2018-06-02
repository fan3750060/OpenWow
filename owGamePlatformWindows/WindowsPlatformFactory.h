#pragma once

class CWindowsPlatformFactory : public IPlatformFactory
{
public:
	CWindowsPlatformFactory();
	~CWindowsPlatformFactory();

	// IPlatformFactory
	IOpenGLAdapter* CreateOpenGLAdapter() override;
	IDebugOutput* CreateDebugOutputLog() override;
	IDebugOutput* CreateDebugOutputConsole() override;
};