#pragma once

// Forward BEGIN
class CInput;
// Forward END

__interface 
	__declspec(uuid("1E876099-AE2B-4CD3-8E7E-E70EE34B528E"))
	IOpenGLAdapter
{
public:
	bool SwapWindowBuffers();
	double GetTime();

	CInput* GetInput();

	void MakeMainContext();
	void MakeThreadContext();

	void SetWindowSize(int32 _width, int32 _height);
	void SetWindowTitle(cstring _title);

	void CreateCursor();
	void ShowCursor();
	void HideCursor();

	void setMousePosition(cvec2 _mousePosition);

	int isExtensionSupported(const char* _extension);
	void* getProcAddress(const char* _procname);
};