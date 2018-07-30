#include "stdafx.h"

// General
#include "DX11_Creator.h"

// Additional
#include "DX11\\RenderDeviceDX11.h"
#include "DX11\\RenderWindowDX11.h"

RenderDevice* DX11_Creator::CreateRenderDevice()
{
	return new RenderDeviceDX11();
}

RenderWindow* DX11_Creator::CreateRenderWindow(HWND hWnd, RenderDevice* device, cstring windowName, int windowWidth, int windowHeight, bool vSync)
{
	RenderDeviceDX11* pDevice = dynamic_cast<RenderDeviceDX11*>(device);

	return new RenderWindowDX11(hWnd, pDevice, windowName, windowWidth, windowHeight, vSync);
}
