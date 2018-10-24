#include <stdafx.h>

// Include render
#include "BufferDX11.h"
#include "ShaderDX11.h"
#include "PipelineStateDX11.h"

// General
#include "MeshDX11.h"

MeshDX11::MeshDX11(ID3D11Device2* pDevice)
	: m_pDevice(pDevice)
	, m_pIndexBuffer(nullptr)
	, m_PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_pMaterial(nullptr)
	, m_pDeviceContext(nullptr)
{
	m_pDevice->GetImmediateContext2(&m_pDeviceContext);
}

MeshDX11::~MeshDX11()
{}

void MeshDX11::AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<Buffer> buffer)
{
	m_VertexBuffers[binding] = buffer;
}

void MeshDX11::SetIndexBuffer(std::shared_ptr<Buffer> buffer)
{
	m_pIndexBuffer = buffer;
}

void MeshDX11::SetPrimitiveTopology(PrimitiveTopology _topology)
{
	switch (_topology)
	{
	case PrimitiveTopology::PointList:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case PrimitiveTopology::LineList:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case PrimitiveTopology::LineStrip:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		break;
	case PrimitiveTopology::TriangleList:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	case PrimitiveTopology::TriangleStrip:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		break;
	}
}

void MeshDX11::SetMaterial(std::shared_ptr<Material> material)
{
	m_pMaterial = material;
}

std::shared_ptr<Material> MeshDX11::GetMaterial() const
{
	return m_pMaterial;
}

void MeshDX11::Render(RenderEventArgs& renderArgs, std::shared_ptr<ConstantBuffer> constantBuffer)
{
	std::shared_ptr<ShaderDX11> pVS;

	// Use the vertex shader to convert the buffer semantics to slot ID's
	if (m_pMaterial)
	{
		m_pMaterial->Bind();

		pVS = std::dynamic_pointer_cast<ShaderDX11>(m_pMaterial->GetShader(Shader::VertexShader));
		if (pVS)
		{
			pVS->GetShaderParameterByName("PerObject").Set(constantBuffer);
		}

		if (pVS)
		{
			for (BufferMap::value_type buffer : m_VertexBuffers)
			{
				BufferBinding binding = buffer.first;
				if (pVS->HasSemantic(binding.Name))
				{
					UINT slotID = pVS->GetSemanticSlot(binding.Name);
					// Bind the vertex buffer to a particular slot ID.
					buffer.second->Bind(slotID, pVS, ShaderParameter::Type::Buffer);
				}
			}
		}
	}
	m_pDeviceContext->IASetPrimitiveTopology(m_PrimitiveTopology);

	if (m_pIndexBuffer != NULL)
	{
		// TOOD: Primitive reset?
		m_pIndexBuffer->Bind(0, pVS, ShaderParameter::Type::Buffer);
		m_pDeviceContext->DrawIndexed(m_pIndexBuffer->GetElementCount(), 0, 0);
		m_pIndexBuffer->UnBind(0, pVS, ShaderParameter::Type::Buffer);
	}
	else
	{
		// We assume we have at least one vertex buffer.
		// If not, then why are we rendering this mesh?
		UINT vertexCount = (*m_VertexBuffers.begin()).second->GetElementCount();
		m_pDeviceContext->Draw(vertexCount, 0);
	}

	if (pVS)
	{
		for (BufferMap::value_type buffer : m_VertexBuffers)
		{
			BufferBinding binding = buffer.first;
			if (pVS->HasSemantic(binding.Name))
			{
				UINT slotID = pVS->GetSemanticSlot(binding.Name);
				// Bind the vertex buffer to a particular slot ID.
				buffer.second->Bind(slotID, pVS, ShaderParameter::Type::Buffer); // TODO: Unbind
			}
		}
	}

	if (m_pMaterial)
	{
		m_pMaterial->Unbind();
	}
}

void MeshDX11::Accept(IVisitor& visitor)
{
	visitor.Visit(*this);
}
