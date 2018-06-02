#pragma once

// Forward BEGIN
class Input;
// Forward END

__interface 
	__declspec(uuid("1E876099-AE2B-4CD3-8E7E-E70EE34B528E"))
	IOpenGLAdapter
{
public:
	bool SwapWindowBuffers();
	double GetTime();

	Input* GetInput();
	HGLRC GetWGLContext();

	//

	void SetWindowSize(int32 _width, int32 _height);
	void SetWindowTitle(cstring _title);

	void ShowCursor();
	void HideCursor();

	void SetMousePosition(cvec2 _mousePosition);
};