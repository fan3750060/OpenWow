#include <stdafx.h>

// Include
#include "BufferOGL.h"
#include "ShaderOGL.h"
#include "PipelineStateOGL.h"

// General
#include "MeshOGL.h"

MeshOGL::MeshOGL()
	: m_pIndexBuffer(nullptr)
	, m_PrimitiveTopology(GL_TRIANGLES)
	, m_pMaterial(nullptr)
	, m_bIsDirty(false)
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
	m_bIsDirty = true;
}

void MeshOGL::SetIndexBuffer(std::shared_ptr<Buffer> buffer)
{
	m_pIndexBuffer = buffer;
	m_bIsDirty = true;
}

void MeshOGL::SetPrimitiveTopology(PrimitiveTopology _topology)
{
	switch (_topology)
	{
	case PrimitiveTopology::PointList:
		m_PrimitiveTopology = GL_POINTS;
		break;
	case PrimitiveTopology::LineList:
		m_PrimitiveTopology = GL_LINES;
		break;
	case PrimitiveTopology::LineStrip:
		m_PrimitiveTopology = GL_LINE_STRIP;
		break;
	case PrimitiveTopology::TriangleList:
		m_PrimitiveTopology = GL_TRIANGLES;
		break;
	case PrimitiveTopology::TriangleStrip:
		m_PrimitiveTopology = GL_TRIANGLE_STRIP;
		break;
	default:
		_ASSERT(false);
	}
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
	PipelineState* pipeline = renderArgs.PipelineState;
	if (pipeline)
	{
		std::shared_ptr<ShaderOGL> pVS = std::dynamic_pointer_cast<ShaderOGL>(pipeline->GetShader(Shader::VertexShader));

		if (m_bIsDirty)
		{
			Commit(pVS);
			m_bIsDirty = false;
		}

		if (m_pMaterial)
		{
			for (auto shader : pipeline->GetShaders())
			{
				m_pMaterial->Bind(shader.second);
			}
		}
	}
	_ASSERT(m_bIsDirty != true);

	glBindVertexArray(m_GLObj);
	{
		if (m_pIndexBuffer != NULL)
		{
			UINT vertexCount = (*m_VertexBuffers.begin()).second->GetElementCount();
			glDrawRangeElements
			(
				m_PrimitiveTopology,
				0,
				vertexCount,
				m_pIndexBuffer->GetElementCount(),
				GL_UNSIGNED_SHORT,
				(char*)0
			);
		}
		else
		{
			UINT vertexCount = (*m_VertexBuffers.begin()).second->GetElementCount();
			glDrawArrays
			(
				m_PrimitiveTopology,
				0,
				vertexCount
			);
		}
	}
	glBindVertexArray(0);
}

void MeshOGL::Accept(IVisitor& visitor)
{
	visitor.Visit(*this);
}

void MeshOGL::Commit(std::weak_ptr<ShaderOGL> _shader)
{
	std::shared_ptr<ShaderOGL> pVS = _shader.lock();

	glBindVertexArray(m_GLObj);
	{
		uint32 newVertexAttribMask = 0;
		uint32 i = 0;
		for (BufferMap::value_type buffer : m_VertexBuffers)
		{
			if (!pVS->HasSemantic(buffer.first.Name))
				continue;

			const InputSemantic& semantic = pVS->GetSemantic(buffer.first.Name);
			UINT             semanticSlot = pVS->GetSemanticSlot(buffer.first.Name);

			// Bind the vertex buffer to a particular slot ID.
			buffer.second->Bind(semanticSlot, pVS, ShaderParameter::Type::Buffer);

			// (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
			glVertexAttribPointer(semanticSlot, semantic.GLSize, semantic.GLType, false, buffer.second->GetElementStride(), (char*)0);
			newVertexAttribMask |= (1 << (i++));
		}

		for (uint32 i = 0; i < 16; ++i)
		{
			uint32 curBit = 1 << i;
			if (newVertexAttribMask & curBit)
				glEnableVertexAttribArray(i);
			else
				glDisableVertexAttribArray(i);
		}

		if (m_pIndexBuffer != NULL)
		{
			m_pIndexBuffer->Bind(0, pVS, ShaderParameter::Type::Buffer);
		}


		for (BufferMap::value_type buffer : m_VertexBuffers)
		{
			if (!pVS->HasSemantic(buffer.first.Name))
				continue;

			UINT semanticSlot = pVS->GetSemanticSlot(buffer.first.Name);

			buffer.second->UnBind(semanticSlot, pVS, ShaderParameter::Type::Buffer);
		}
	}
	glBindVertexArray(0);
}
