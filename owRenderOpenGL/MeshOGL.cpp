#include <stdafx.h>

// Include render
#include "BufferOGL.h"
#include "ShaderOGL.h"
#include "PipelineStateOGL.h"

// General
#include "MeshOGL.h"

MeshOGL::MeshOGL()
	: m_pIndexBuffer(nullptr)
	, m_pMaterial(nullptr)
{
	glGenVertexArrays(1, &m_GLObj);
}

MeshOGL::~MeshOGL()
{
	if (m_GLObj != 0)
	{
		glDeleteVertexArrays(1, &m_GLObj);
		m_GLObj = 0;
	}
}

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
	glBindVertexArray(m_GLObj);

	std::shared_ptr<ShaderOGL> pVS;

	// Use the vertex shader to convert the buffer semantics to slot ID's
	PipelineState* pipeline = renderArgs.PipelineState;
	if (pipeline)
	{
		pVS = std::dynamic_pointer_cast<ShaderOGL>(pipeline->GetShader(Shader::VertexShader));

		if (pVS)
		{
			for (BufferMap::value_type buffer : m_VertexBuffers)
			{
				const BufferBinding& binding = pVS->GetSemantic(buffer.first.Name);

				UINT slotID = pVS->GetSlotIDBySemantic(buffer.first.Name);
				// Bind the vertex buffer to a particular slot ID.
				buffer.second->Bind(slotID, pVS, ShaderParameter::Type::Buffer);

				// (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
				glVertexAttribPointer(slotID, binding.GLSize, binding.GLType, false, buffer.second->GetElementStride(), (char*)0);

				if (pVS->HasSemantic(buffer.first.Name))
				{
					glEnableVertexAttribArray(binding.Index);
				}
				else
				{
					glDisableVertexAttribArray(binding.Index);
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

	if (m_pIndexBuffer != NULL)
	{
		m_pIndexBuffer->Bind(0, pVS, ShaderParameter::Type::Buffer);

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

		m_pIndexBuffer->UnBind(0, pVS, ShaderParameter::Type::Buffer);
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
			if (pVS->HasSemantic(buffer.first.Name))
			{
				UINT slotID = pVS->GetSlotIDBySemantic(buffer.first.Name);
				// Bind the vertex buffer to a particular slot ID.
				buffer.second->UnBind(slotID, pVS, ShaderParameter::Type::Buffer);
			}
		}
	}

	glBindVertexArray(0);
}

void MeshOGL::Accept(IVisitor& visitor)
{
	visitor.Visit(*this);
}
