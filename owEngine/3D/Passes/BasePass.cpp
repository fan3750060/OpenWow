#include <stdafx.h>

// Include
#include <Application.h>
#include <3D//Scene3D.h>
#include <3D//SceneNode3D.h>

// General
#include "BasePass.h"

BasePass::BasePass()
	: m_pRenderEventArgs(nullptr)
	, m_RenderDevice(_RenderDevice)
{

}

BasePass::BasePass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: m_pRenderEventArgs(nullptr)
	, m_Scene(scene)
	, m_Pipeline(pipeline)
	, m_RenderDevice(_RenderDevice)
{

}

BasePass::~BasePass()
{
}

void BasePass::PreRender(Render3DEventArgs& e)
{
	e.PipelineState = m_Pipeline.get();
	SetRenderEventArgs(e);

	if (m_Pipeline)
	{
		m_Pipeline->Bind();
	}
}

void BasePass::Render(Render3DEventArgs& e)
{
	if (m_Scene)
	{
		m_Scene->Accept(*this);
	}
}

void BasePass::PostRender(Render3DEventArgs& e)
{
	if (m_Pipeline)
	{
		m_Pipeline->UnBind();
	}
}

// Inherited from Visitor

bool BasePass::Visit(SceneNode3D& node)
{
	Object& nodeAsObject = reinterpret_cast<Object&>(node);
	m_pRenderEventArgs->Node = &nodeAsObject;

	const Camera* camera = GetRenderEventArgs().Camera;
	if (camera)
	{
		PerObject perObjectData;
		perObjectData.Model               = node.GetWorldTransfom();
		perObjectData.ModelView           = camera->GetViewMatrix()       * perObjectData.Model;
		perObjectData.ModelViewProjection = camera->GetProjectionMatrix() * perObjectData.ModelView;

		// Update the constant buffer data
		SetPerObjectConstantBufferData(perObjectData);

		node.UpdateCamera(camera);

		return true;
	}

	return false;
}

bool BasePass::Visit(IMesh& mesh)
{
	if (m_pRenderEventArgs)
	{
		return mesh.Render(*m_pRenderEventArgs, m_PerObjectConstantBuffer);
	}

	return false;
}

bool BasePass::Visit(CLight3D& light)
{
	return false;
}

//----------------------------------------------------------------------

void BasePass::SetRenderEventArgs(Render3DEventArgs& e)
{
	m_pRenderEventArgs = &e;
}

Render3DEventArgs& BasePass::GetRenderEventArgs() const
{
	assert(m_pRenderEventArgs);
	return *m_pRenderEventArgs;
}

//----------------------------------------------------------------------

std::shared_ptr<IRenderDevice> BasePass::GetRenderDevice() const
{
	return m_RenderDevice.lock();
}

std::shared_ptr<PipelineState> BasePass::GetPipelineState() const
{
	return m_Pipeline;
}
