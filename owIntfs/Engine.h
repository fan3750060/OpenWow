#pragma once

// Forward BEGIN
__interface IOpenGLAdapter;
// Forward END

__interface
	__declspec(uuid("9805BCA8-793A-4071-8C4A-1A1F87F0DFC3"))
	IEngine : public IManager
{
	bool Tick();
	IOpenGLAdapter* GetAdapter();
	uint8 GetArgumentsCount() const;
	string GetArgument(uint8 index) const;
	uint32 GetFPS() const;
	void SetNeedExit();
	bool IsNeedExit() const;
};