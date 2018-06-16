#pragma once



class OpenGLAdapter_GLFW : public IOpenGLAdapter
{
public:
	OpenGLAdapter_GLFW();
	~OpenGLAdapter_GLFW();

	// IOpenGLAdapter
	bool SwapWindowBuffers() override;
	double GetTime() override;
	Input* GetInput() override;

	HGLRC GetMainCont() override;
	HGLRC GetThreadCont() override;
	void MakeContextMainThread() override;
	void MakeCurrent() override;

	void SetWindowSize(int32 _width, int32 _height) override;
	void SetWindowTitle(cstring _title) override;
	void ShowCursor() override;
	void HideCursor() override;
	void SetMousePosition(cvec2 _mousePosition) override;
};
