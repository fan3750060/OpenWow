#include "stdafx.h"

// General
#include "owRenderOpenGL.h"

// Additional
#include "RenderDeviceOGL.h"
#include "RenderWindowOGL.h"

CBaseManager* _BaseManager = nullptr;

RenderDevice* CreateRenderDeviceOGL(CBaseManager* _baseManager)
{
	_BaseManager = _baseManager;
	return new RenderDeviceOGL();
}

RenderWindow* CreateRenderWindowOGL(HWND hWnd, RenderDevice* device, cstring windowName, int windowWidth, int windowHeight, bool vSync)
{
	RenderDeviceOGL* pDevice = dynamic_cast<RenderDeviceOGL*>(device);
	assert1(pDevice != NULL);

	return new RenderWindowOGL(hWnd, pDevice, windowName, windowWidth, windowHeight, vSync);
}