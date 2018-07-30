#pragma once

#include "RenderDevice.h"

class DX11_Creator
{
public:
	static RenderDevice* CreateRenderDevice();
	static RenderWindow* CreateRenderWindow(HWND hWnd, RenderDevice* device, cstring windowName, int windowWidth, int windowHeight, bool vSync);
};