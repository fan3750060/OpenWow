#pragma once

// Common includes
#include "Common.h"

OW_RENDER_DX11_API IRenderDevice* CreateRenderDeviceDX11(CBaseManager* _BaseManager);
OW_RENDER_DX11_API RenderWindow* CreateRenderWindowDX11(HWND hWnd, IRenderDevice* device, cstring windowName, int windowWidth, int windowHeight, bool vSync);