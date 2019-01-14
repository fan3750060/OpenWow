#pragma once

#include "RenderUIPass.h"

class IRenderDevice;
class Shader;
class ConstantBuffer;
class Camera;
class UIScene;
class PipelineState;
class Query;

// Base pass provides implementations for functions used by most passes.
class BaseUIPass : public IRenderUIPass
{
	typedef IRenderUIPass base;

public:
	BaseUIPass();
	BaseUIPass(std::shared_ptr<UIScene> uiScene, std::shared_ptr<PipelineState> pipeline);
	virtual ~BaseUIPass();

	// Enable or disable the pass. If a pass is disabled, the technique will skip it.
	virtual void SetEnabled(bool enabled);
	virtual bool IsEnabled() const;

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void RenderUI(RenderUIEventArgs& e);

	// Inherited from Visitor
	virtual bool Visit(SceneNode& node);
	virtual bool Visit(UINode& node);
	virtual bool Visit(IMesh& mesh);

protected:
	// PerObject constant buffer data.
	__declspec(align(16)) struct PerObject
	{
		glm::mat4 ModelOrtho;
		glm::mat4 Model;
	};

public:
	void SetRenderUIEventArgs(RenderUIEventArgs& e);
	RenderUIEventArgs& GetRenderUIEventArgs() const;

	IRenderDevice* GetRenderDevice() const;

	void SetPerObjectConstantBufferData(PerObject& perObjectData);
	std::shared_ptr<ConstantBuffer> GetPerObjectConstantBuffer() const;

private:
	bool m_Enabled;
	PerObject* m_PerObjectData;
	std::shared_ptr<ConstantBuffer> m_PerObjectConstantBuffer;

	RenderUIEventArgs* m_pRenderEventArgs;

	// The pipeline state that should be used to render this pass.
	std::shared_ptr<PipelineState> m_Pipeline;

	// The scene to render.
	std::shared_ptr<UIScene> m_UIScene;

	IRenderDevice* m_RenderDevice;
};