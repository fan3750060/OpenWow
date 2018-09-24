#pragma once

// Common includes
#include "Common.h"

OW_RENDER_OPENGL_API RenderDevice* CreateRenderDeviceOGL(CBaseManager* _BaseManager);
OW_RENDER_OPENGL_API RenderWindow* CreateRenderWindowOGL(HWND hWnd, RenderDevice* device, cstring windowName, int windowWidth, int windowHeight, bool vSync);