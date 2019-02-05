#pragma once

#include "RenderUIPass.h"

class IRenderDevice;
class Shader;
class ConstantBuffer;
class SceneUI;
class PipelineState;
class Query;

// Base pass provides implementations for functions used by most passes.
class BaseUIPass : public IRenderUIPass
{
	typedef IRenderUIPass base;

public:
	BaseUIPass();
	BaseUIPass(std::shared_ptr<SceneUI> uiScene, std::shared_ptr<PipelineState> pipeline);
	virtual ~BaseUIPass();

	// Enable or disable the pass. If a pass is disabled, the technique will skip it.
	virtual void SetEnabled(bool enabled);
	virtual bool IsEnabled() const;

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void RenderUI(RenderUIEventArgs& e);

private:
	bool m_Enabled;

	// The pipeline state that should be used to render this pass.
	std::shared_ptr<PipelineState> m_Pipeline;

	// The scene to render.
	std::shared_ptr<SceneUI> m_UIScene;

	std::weak_ptr<IRenderDevice> m_RenderDevice;
};