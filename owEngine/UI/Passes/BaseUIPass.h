#pragma once

#include "RenderUIPass.h"

#include "../UIScene.h"
#include "../UIBaseNode.h"

// Base pass provides implementations for functions used by most passes.
class BaseUIPass : public IRenderUIPass
{
	typedef IRenderUIPass base;

public:
	BaseUIPass();
	BaseUIPass(std::shared_ptr<CUIScene> uiScene, std::shared_ptr<PipelineState> pipeline);
	virtual ~BaseUIPass();

	// Enable or disable the pass. If a pass is disabled, the technique will skip it.
	virtual void SetEnabled(bool enabled);
	virtual bool IsEnabled() const;

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void RenderUI(RenderUIEventArgs& e);

	virtual void UpdateViewport(Viewport _viewport) override final;

private:
	bool m_Enabled;

	// The pipeline state that should be used to render this pass.
	std::shared_ptr<PipelineState> m_Pipeline;

	// The scene to render.
	std::shared_ptr<CUIScene> m_UIScene;

	std::weak_ptr<IRenderDevice> m_RenderDevice;
};