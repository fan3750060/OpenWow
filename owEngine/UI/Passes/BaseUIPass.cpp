#include <stdafx.h>

#include <Application.h>
#include <UIScene.h>
#include <UINode.h>

// General
#include "BaseUIPass.h"

BaseUIPass::BaseUIPass()
	: m_Enabled(true)
	, m_pRenderEventArgs(nullptr)
	, m_RenderDevice(_RenderDevice)
{
	m_PerObjectData = (PerObject*)_aligned_malloc(sizeof(PerObject), 16);
	m_PerObjectConstantBuffer = m_RenderDevice.lock()->CreateConstantBuffer(PerObject());
}

BaseUIPass::BaseUIPass(std::shared_ptr<UIScene> uiScene, std::shared_ptr<PipelineState> pipeline)
	: m_Enabled(true)
	, m_pRenderEventArgs(nullptr)
	, m_UIScene(uiScene)
	, m_Pipeline(pipeline)
	, m_RenderDevice(_RenderDevice)
{
	m_PerObjectData = (PerObject*)_aligned_malloc(sizeof(PerObject), 16);
	m_PerObjectConstantBuffer = m_RenderDevice.lock()->CreateConstantBuffer(PerObject());
}

BaseUIPass::~BaseUIPass()
{
	_aligned_free(m_PerObjectData);
	m_RenderDevice.lock()->DestroyConstantBuffer(m_PerObjectConstantBuffer);
}

// Enable or disable the pass. If a pass is disabled, the technique will skip it.
void BaseUIPass::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}

bool BaseUIPass::IsEnabled() const
{
	return m_Enabled;
}

void BaseUIPass::RenderUI(RenderUIEventArgs& e)
{
	SetRenderUIEventArgs(e);

	if (m_Pipeline)
	{
		m_Pipeline->Bind();
	}

	if (m_UIScene)
	{
		m_UIScene->Accept(*this);
	}

	if (m_Pipeline)
	{
		m_Pipeline->UnBind();
	}
}

// Inherited from Visitor

bool BaseUIPass::Visit(SceneNode & node)
{
	fail1();
	return false;
}

bool BaseUIPass::Visit(UINode& node)
{
	const Viewport* viewport = GetRenderUIEventArgs().Viewport;
	if (viewport)
	{
		PerObject perObjectData;
		perObjectData.Model      = node.GetWorldTransfom();
		perObjectData.ModelOrtho = viewport->OrthoMatrix * perObjectData.Model;

		// Update the constant buffer data
		SetPerObjectConstantBufferData(perObjectData);

		node.UpdateViewport(viewport);

		return true;
	}

	return false;
}

bool BaseUIPass::Visit(IMesh& mesh)
{
	std::shared_ptr<const Material> pMaterial = mesh.GetMaterial();
	if (pMaterial && m_pRenderEventArgs)
	{
		RenderEventArgs args(GetRenderUIEventArgs().Caller, GetRenderUIEventArgs().ElapsedTime, GetRenderUIEventArgs().TotalTime, GetRenderUIEventArgs().FrameCounter);
		return mesh.Render(args, m_PerObjectConstantBuffer);
	}

	return false;
}

//----------------------------------------------------------------------

void BaseUIPass::SetRenderUIEventArgs(RenderUIEventArgs& e)
{
	m_pRenderEventArgs = &e;
}

RenderUIEventArgs& BaseUIPass::GetRenderUIEventArgs() const
{
	assert(m_pRenderEventArgs);
	return *m_pRenderEventArgs;
}

//----------------------------------------------------------------------

std::shared_ptr<IRenderDevice> BaseUIPass::GetRenderDevice() const
{
	return m_RenderDevice.lock();
}

//----------------------------------------------------------------------

void BaseUIPass::SetPerObjectConstantBufferData(PerObject& perObjectData)
{
	m_PerObjectConstantBuffer->Set(perObjectData);
}

std::shared_ptr<ConstantBuffer> BaseUIPass::GetPerObjectConstantBuffer() const
{
	return m_PerObjectConstantBuffer;
}
