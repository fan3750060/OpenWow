#pragma once

class TextureDX11;
class RenderTargetDX11;

class RenderWindowDX11 : public RenderWindow
{
	typedef RenderWindow base;
public:
	RenderWindowDX11(HWND hWnd, RenderDeviceDX11* device, cstring windowName, int windowWidth, int windowHeight, bool vSync);
	virtual ~RenderWindowDX11();

	virtual void Present();

	virtual std::shared_ptr<RenderTarget> GetRenderTarget();

protected:
	virtual void CreateSwapChain();
	virtual void OnPreRender(RenderEventArgs& e);
	virtual void OnPostRender(RenderEventArgs& e);

	virtual void OnMouseMoved(MouseMotionEventArgs& e);
	virtual void OnMouseLeave(EventArgs& e);

	virtual void OnResize(ResizeEventArgs& e);

	virtual void OnTerminate(EventArgs& e);

	virtual void ResizeSwapChainBuffers(uint32_t width, uint32_t height);

private:
	bool m_bIsMouseTracking; // Used to capture mouse enter/leave events.

	RenderDeviceDX11* m_Device;

	// Used to enable multisampling AA
	DXGI_SAMPLE_DESC m_SampleDesc; // = { 1, 0 };

	ATL::CComPtr<ID3D11Device2> m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;
	ATL::CComPtr<IDXGISwapChain2> m_pSwapChain;
	ATL::CComPtr<ID3D11Texture2D> m_pBackBuffer;

	std::shared_ptr<RenderTargetDX11> m_RenderTarget;

	// If the window has to be resized, delay the resizing of the swap chain until the prerender function.
	bool m_bResizePending;
};