#pragma once

class IRenderUIPass : public Object
{
public:
	virtual void SetEnabled(bool enabled) = 0;
	virtual bool IsEnabled() const = 0;

	virtual void RenderUI(RenderUIEventArgs& e) = 0;

	// Update viewport
	virtual void UpdateViewport(Viewport _viewport) = 0;
};