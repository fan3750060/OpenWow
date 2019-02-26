#pragma once

// Forward BEGIN
class IRenderDevice;
class RenderWindow;
// Forward END

struct IApplication
{
	virtual ~IApplication() = 0 {};

	virtual std::shared_ptr<IRenderDevice>  GetRenderDevice() = 0;
	virtual std::shared_ptr<RenderWindow>   GetRenderWindow() = 0;
};
