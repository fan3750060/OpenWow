#pragma once

// Forward BEGIN
__interface IOpenGLAdapter;
__interface IDebugOutput;
// Forward END

__interface 
	__declspec(uuid("D6BEC901-09F0-4973-BBA7-DAFDC00BCF70"))
	IPlatformFactory : public IManager
{
	IOpenGLAdapter* CreateOpenGLAdapter();
};