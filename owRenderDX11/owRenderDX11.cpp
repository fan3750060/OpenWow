#include "stdafx.h"


// General
#include "owRenderDX11.h"


// Additional
#include "RenderDeviceDX11.h"
#include "RenderWindowDX11.h"


// Statics
std::shared_ptr<CBaseManager> _BaseManager = nullptr;


// Functions
std::shared_ptr<IRenderDevice> CreateRenderDeviceDX11(std::shared_ptr<CBaseManager> _baseManager)
{
	_BaseManager = _baseManager;
	return std::make_shared<RenderDeviceDX11>();
}

std::shared_ptr<RenderWindow> CreateRenderWindowDX11(HWND hWnd, std::shared_ptr<IRenderDevice> device, cstring windowName, int windowWidth, int windowHeight, bool vSync)
{
	std::shared_ptr<RenderDeviceDX11> pDevice = std::dynamic_pointer_cast<RenderDeviceDX11, IRenderDevice>(device);
	assert1(pDevice != NULL);

	return std::make_shared<RenderWindowDX11>(hWnd, pDevice, windowName, windowWidth, windowHeight, vSync);
}