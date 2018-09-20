#include <stdafx.h>

// General
#include "BufferOGL.h"

// Additional
#include "OpenGL.h"

static const uint32 bufferMappingTypes[3] = { GL_MAP_READ_BIT, GL_MAP_WRITE_BIT, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT };

BufferOGL::BufferOGL(UINT bindFlags, const void* data, size_t count, UINT stride)
	: m_uiStride(stride)
	, m_BindFlags(bindFlags)
	, m_uiCount((UINT)count)
	, m_bIsBound(false)
{
	glGenBuffers(1, &m_GLObj);
	assert1(m_GLObj != 0);
	glBindBuffer(m_BindFlags, m_GLObj);
	glBufferData(m_BindFlags, count * stride, data, /*_isDynamic ? */GL_DYNAMIC_DRAW/* : GL_STATIC_DRAW*/);
	glBindBuffer(m_BindFlags, 0);
}

BufferOGL::~BufferOGL()
{
	glDeleteBuffers(1, &m_GLObj);
}

bool BufferOGL::Bind(uint32 id, Shader::ShaderType shaderType, ShaderParameter::Type parameterType)
{
	/*assert(m_pDeviceContext);

	ID3D11Buffer* buffers[] = { m_pBuffer };
	UINT offsets[] = { 0 };
	UINT strides[] = { m_uiStride };

	switch (m_BindFlags)
	{
	case D3D11_BIND_VERTEX_BUFFER:
		m_pDeviceContext->IASetVertexBuffers(id, 1, buffers, strides, offsets);
		m_bIsBound = true;
		break;
	case D3D11_BIND_INDEX_BUFFER:
		m_pDeviceContext->IASetIndexBuffer(m_pBuffer, m_uiStride == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
		m_bIsBound = true;
		break;
	default:
		throw std::exception("BufferOGL::Bind: Unimplemented buffer type.");
		// return false;
		break;
	}*/

	throw std::exception("Not implemented!");
}

void BufferOGL::UnBind(uint32 id, Shader::ShaderType shaderType, ShaderParameter::Type parameterType)
{
	glBindBuffer(m_BindFlags, 0);
	/*ID3D11Buffer* buffers[] = { nullptr };

	switch (m_BindFlags)
	{
	case D3D11_BIND_VERTEX_BUFFER:
		m_pDeviceContext->IASetVertexBuffers(id, 1, buffers, nullptr, nullptr);
		m_bIsBound = true;
		break;
	case D3D11_BIND_INDEX_BUFFER:
		m_pDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		m_bIsBound = true;
		break;
	default:
		throw std::exception("BufferOGL::Bind: Unimplemented buffer type.");
		// return false;
		break;
	}*/

	throw std::exception("Not implemented!");
}

void BufferOGL::Copy(std::shared_ptr<Buffer> other)
{
	/*std::shared_ptr<BufferOGL> srcBuffer = std::dynamic_pointer_cast<BufferOGL>(other);

	if (srcBuffer && srcBuffer.get() != this &&
		m_uiCount * m_uiStride == srcBuffer->m_uiCount * srcBuffer->m_uiStride)
	{
		m_pDeviceContext->CopyResource(m_pBuffer, srcBuffer->m_pBuffer);
	}
	else
	{
		Log::Error("Source buffer is not compatible with this buffer.");
	}*/
	throw std::exception("Not implemented!");
}

Buffer::BufferType BufferOGL::GetType() const
{
	switch (m_BindFlags)
	{
	case GL_ARRAY_BUFFER:
		return Buffer::VertexBuffer;
		break;
	case GL_ELEMENT_ARRAY_BUFFER:
		return Buffer::IndexBuffer;
		break;
	/*case GL_CONSTA: /-???-/
		return Buffer::ConstantBuffer;
		break;*/
	default:
		return Buffer::Unknown;
		break;
	}
}

uint32 BufferOGL::GetElementCount() const
{
	return m_uiCount;
}