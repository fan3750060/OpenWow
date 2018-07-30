#include "stdafx.h"

// General
#include "R_TextureBuffer.h"

// Additional
#include "OpenGL.h"

R_TextureBuffer::R_TextureBuffer(RenderDevice* _RenderDevice) :
	m_Buffer(nullptr),
	m_GLFmt(0),
	m_GLTexID(0),
	m_RenderDevice(_RenderDevice)
{}

R_TextureBuffer::R_TextureBuffer(const R_TextureBuffer & _other)
{
	if (this != &_other)
	{
		m_Buffer = _other.m_Buffer;
		m_GLFmt = _other.m_GLFmt;
		m_GLTexID = _other.m_GLTexID;
		m_RenderDevice = _other.m_RenderDevice;
	}
}

R_TextureBuffer::~R_TextureBuffer()
{
	glDeleteTextures(1, &m_GLTexID);
}

void R_TextureBuffer::createTextureBuffer(R_TextureFormats::List _format, uint32 _bufSize, const void* _data, bool _isDynamic)
{
	m_Buffer = make_shared<R_Buffer>(m_RenderDevice);
	m_Buffer->createBuffer(GL_TEXTURE_BUFFER, _bufSize, _data, _isDynamic);

	glGenTextures(1, &m_GLTexID);
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_BUFFER, m_GLTexID);

	switch (_format)
	{
	case R_TextureFormats::RGBA8:
		m_GLFmt = GL_RGBA8;
		break;
	case R_TextureFormats::RGBA16F:
		m_GLFmt = GL_RGBA16F;
		break;
	case R_TextureFormats::RGBA32F:
		m_GLFmt = GL_RGBA32F;
		break;
	case R_TextureFormats::R32:
		m_GLFmt = GL_R32F;
		break;
	case R_TextureFormats::RG32:
		m_GLFmt = GL_RG32F;
		break;
	default:
		fail1();
		break;
	};

	// bind texture to buffer
	glTexBuffer(GL_TEXTURE_BUFFER, m_GLFmt, m_Buffer->m_GLObj);

	glBindTexture(GL_TEXTURE_BUFFER, 0);
	if (m_RenderDevice->m_State.m_TextureSlot[15].m_Texture)
	{
		glBindTexture(m_RenderDevice->m_State.m_TextureSlot[15].m_Texture->m_Type, m_RenderDevice->m_State.m_TextureSlot[15].m_Texture->m_GLObj);
	}
}