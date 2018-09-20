#pragma once

class TextureOGL;
class RenderTargetOGL;

class RenderWindowOGL : public RenderWindow
{
public:
	typedef RenderWindow base;

	RenderWindowOGL(HWND hWnd, RenderDeviceOGL* device, cstring windowName, int windowWidth, int windowHeight, bool vSync);
	virtual ~RenderWindowOGL();

	virtual void ShowWindow();
	virtual void HideWindow();
	virtual void CloseWindow();

	virtual void Present();

	virtual std::shared_ptr<RenderTarget> GetRenderTarget();

protected:
	virtual void OnPreRender(RenderEventArgs& e);
	virtual void OnPostRender(RenderEventArgs& e);

	virtual void OnMouseMoved(MouseMotionEventArgs& e);
	virtual void OnMouseLeave(EventArgs& e);

	virtual void OnResize(ResizeEventArgs& e);

	virtual void OnTerminate(EventArgs& e);

	virtual void ResizeSwapChainBuffers(uint32_t width, uint32_t height);

private:
	HDC m_HDC;

private:
	bool m_bIsMouseTracking; // Used to capture mouse enter/leave events.

	HWND m_hWindow;
	RenderDeviceOGL* m_Device;

	std::shared_ptr<RenderTargetOGL> m_RenderTarget;

	// If the window has to be resized, delay the resizing of the swap chain until the prerender function.
	bool m_bResizePending;
};