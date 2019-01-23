#include <stdafx.h>

#include <Application.h>
#include <Scene.h>
#include <SceneNode.h>

// General
#include "BasePass.h"

BasePass::BasePass()
	: m_pRenderEventArgs(nullptr)
	, m_RenderDevice(_RenderDevice)
{
	m_PerObjectData = (PerObject*)_aligned_malloc(sizeof(PerObject), 16);
	m_PerObjectConstantBuffer = m_RenderDevice->CreateConstantBuffer(PerObject());
}

BasePass::BasePass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline)
	: m_pRenderEventArgs(nullptr)
	, m_Scene(scene)
	, m_Pipeline(pipeline)
	, m_RenderDevice(_RenderDevice)
{
	m_PerObjectData = (PerObject*)_aligned_malloc(sizeof(PerObject), 16);
	m_PerObjectConstantBuffer = m_RenderDevice->CreateConstantBuffer(PerObject());
}

BasePass::~BasePass()
{
	_aligned_free(m_PerObjectData);
	m_RenderDevice->DestroyConstantBuffer(m_PerObjectConstantBuffer);
}

void BasePass::PreRender(RenderEventArgs& e)
{
	e.PipelineState = m_Pipeline.get();
	SetRenderEventArgs(e);

	if (m_Pipeline)
	{
		m_Pipeline->Bind();
	}
}

void BasePass::Render(RenderEventArgs& e)
{
	if (m_Scene)
	{
		m_Scene->Accept(*this);
	}
}

void BasePass::PostRender(RenderEventArgs& e)
{
	if (m_Pipeline)
	{
		m_Pipeline->UnBind();
	}
}

// Inherited from Visitor

bool BasePass::Visit(SceneNode& node)
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
	std::shared_ptr<const Material> pMaterial = mesh.GetMaterial();
	if (pMaterial && m_pRenderEventArgs)
	{
		return mesh.Render(*m_pRenderEventArgs, m_PerObjectConstantBuffer);
	}

	return false;
}

//----------------------------------------------------------------------

void BasePass::SetRenderEventArgs(RenderEventArgs& e)
{
	m_pRenderEventArgs = &e;
}

RenderEventArgs& BasePass::GetRenderEventArgs() const
{
	assert(m_pRenderEventArgs);
	return *m_pRenderEventArgs;
}

//----------------------------------------------------------------------

IRenderDevice* BasePass::GetRenderDevice() const
{
	return m_RenderDevice;
}

std::shared_ptr<PipelineState> BasePass::GetPipelineState() const
{
	return m_Pipeline;
}

//----------------------------------------------------------------------

void BasePass::SetPerObjectConstantBufferData(PerObject& perObjectData)
{
	m_PerObjectConstantBuffer->Set(perObjectData);
}

std::shared_ptr<ConstantBuffer> BasePass::GetPerObjectConstantBuffer() const
{
	return m_PerObjectConstantBuffer;
}
