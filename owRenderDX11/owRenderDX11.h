#pragma once

// Common includes
#include "Common.h"

OW_RENDER_DX11_API std::shared_ptr<IRenderDevice> CreateRenderDeviceDX11(std::shared_ptr<CBaseManager> _BaseManager);
OW_RENDER_DX11_API std::shared_ptr<RenderWindow>  CreateRenderWindowDX11(HWND hWnd, std::shared_ptr<IRenderDevice> device, cstring windowName, int windowWidth, int windowHeight, bool vSync);