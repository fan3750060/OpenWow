#include "stdafx.h"

// General
#include "MeshWrapper.h"

MeshWrapper::MeshWrapper(std::shared_ptr<IMesh> _mesh) :
	m_Mesh(_mesh)
{
}

MeshWrapper::~MeshWrapper()
{
}

SceneNodeTypes MeshWrapper::GetType() const
{
	return m_Mesh->GetType();
}

void MeshWrapper::SetType(SceneNodeTypes type)
{
	m_Mesh->SetType(type);
}

void MeshWrapper::AddVertexBuffer(const BufferBinding & binding, std::shared_ptr<Buffer> buffer)
{
	m_Mesh->AddVertexBuffer(binding, buffer);
}

void MeshWrapper::SetIndexBuffer(std::shared_ptr<Buffer> buffer)
{
	m_Mesh->SetIndexBuffer(buffer);
}

void MeshWrapper::SetPrimitiveTopology(PrimitiveTopology _topology)
{
	m_Mesh->SetPrimitiveTopology(_topology);
}

void MeshWrapper::SetMaterial(std::shared_ptr<const Material> material)
{
	m_Mesh->SetMaterial(material);
}

std::shared_ptr<const Material> MeshWrapper::GetMaterial() const
{
	return m_Mesh->GetMaterial();
}

void MeshWrapper::Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation, UINT indexCnt)
{
	m_Mesh->Render(renderEventArgs, perObject, indexStartLocation, indexCnt);
}

void MeshWrapper::Accept(IVisitor& visitor)
{
	visitor.Visit(*this);
}
