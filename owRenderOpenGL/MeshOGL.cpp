#include <stdafx.h>

// Include render
#include "BufferOGL.h"
#include "ShaderOGL.h"
#include "PipelineStateOGL.h"

// General
#include "MeshOGL.h"

// Additional
#include "OpenGL.h"

MeshOGL::MeshOGL()
	: m_pIndexBuffer(nullptr)
	, m_pMaterial(nullptr)
{
}

MeshOGL::~MeshOGL()
{}

void MeshOGL::AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<Buffer> buffer)
{
	m_VertexBuffers[binding] = buffer;
}

void MeshOGL::SetIndexBuffer(std::shared_ptr<Buffer> buffer)
{
	m_pIndexBuffer = buffer;
}

void MeshOGL::SetMaterial(std::shared_ptr<Material> material)
{
	m_pMaterial = material;
}

std::shared_ptr<Material> MeshOGL::GetMaterial() const
{
	return m_pMaterial;
}

void MeshOGL::Render(RenderEventArgs& renderArgs)
{
	std::shared_ptr<ShaderOGL> pVS;

	// Clone this mesh's material in case we want to override the 
	// shaders in the mesh's default material.
	//Material material( *m_pMaterial );

	// Use the vertex shader to convert the buffer semantics to slot ID's
	PipelineState* pipeline = renderArgs.PipelineState;
	if (pipeline)
	{
		pVS = std::dynamic_pointer_cast<ShaderOGL>(pipeline->GetShader(Shader::VertexShader));

		if (pVS)
		{
			for (BufferMap::value_type buffer : m_VertexBuffers)
			{
				BufferBinding binding = buffer.first;
				if (pVS->HasSemantic(binding))
				{
					UINT slotID = pVS->GetSlotIDBySemantic(binding);
					// Bind the vertex buffer to a particular slot ID.
					buffer.second->Bind(slotID, Shader::VertexShader, ShaderParameter::Type::Buffer);
				}
			}
		}

		if (m_pMaterial)
		{
			for (auto shader : pipeline->GetShaders())
			{
				m_pMaterial->Bind(shader.second);
			}
		}
	}

	// TODO: The primitive topology should be a parameter.
	// Or we have to have index buffers/vertex buffers for each primitive type...
	//m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (m_pIndexBuffer != NULL)
	{
		// TOOD: Primitive reset?
		m_pIndexBuffer->Bind(0, Shader::VertexShader, ShaderParameter::Type::Buffer);

		UINT vertexCount = (*m_VertexBuffers.begin()).second->GetElementCount();
		glDrawRangeElements
		(
			GL_TRIANGLES,
			0,
			vertexCount,
			m_pIndexBuffer->GetElementCount(),
			GL_UNSIGNED_SHORT,
			(char*)0
		);

		m_pIndexBuffer->UnBind(0, Shader::VertexShader, ShaderParameter::Type::Buffer);
	}
	else
	{
		// We assume we have at least one vertex buffer.
		// If not, then why are we rendering this mesh?
		UINT vertexCount = (*m_VertexBuffers.begin()).second->GetElementCount();
		glDrawArrays
		(
			GL_TRIANGLES,
			0,
			vertexCount
		);
	}

	if (pVS)
	{
		for (BufferMap::value_type buffer : m_VertexBuffers)
		{
			BufferBinding binding = buffer.first;
			if (pVS->HasSemantic(binding))
			{
				UINT slotID = pVS->GetSlotIDBySemantic(binding);
				// Bind the vertex buffer to a particular slot ID.
				buffer.second->Bind(slotID, Shader::VertexShader, ShaderParameter::Type::Buffer);
			}
		}
	}

}

void MeshOGL::Accept(IVisitor& visitor)
{
	visitor.Visit(*this);
}
