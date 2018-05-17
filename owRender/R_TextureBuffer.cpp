#include "stdafx.h"

// General
#include "R_TextureBuffer.h"

// Additional
#include "OpenGL.h"

R_TextureBuffer* R_TextureBuffer::createTextureBuffer(R_TextureFormats::List format, uint32 bufSize, const void *data, bool _isDynamic)
{
	this->bufObj = new R_Buffer(m_RenderDevice);
	this->bufObj = this->bufObj->createBuffer(GL_TEXTURE_BUFFER, bufSize, data, _isDynamic);

	glGenTextures(1, &this->glTexID);
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_BUFFER, this->glTexID);

	switch (format)
	{
	case R_TextureFormats::RGBA8:
		this->glFmt = GL_RGBA8;
		break;
	case R_TextureFormats::RGBA16F:
		this->glFmt = GL_RGBA16F;
		break;
	case R_TextureFormats::RGBA32F:
		this->glFmt = GL_RGBA32F;
		break;
	case R_TextureFormats::R32:
		this->glFmt = GL_R32F;
		break;
	case R_TextureFormats::RG32:
		this->glFmt = GL_RG32F;
		break;
	default:
		fail1();
		break;
	};

	// bind texture to buffer
	glTexBuffer(GL_TEXTURE_BUFFER, this->glFmt, this->bufObj->glObj);

	glBindTexture(GL_TEXTURE_BUFFER, 0);
	if (m_RenderDevice->_texSlots[15].texObj)
	{
		glBindTexture(m_RenderDevice->_texSlots[15].texObj->type, m_RenderDevice->_texSlots[15].texObj->glObj);
	}

	return this;
}

void R_TextureBuffer::destroyTextureBuffer()
{
	destroyBuffer();
	glDeleteTextures(1, &this->glTexID);
}