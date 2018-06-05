#include "stdafx.h"

// General
#include "R_RenderBuffer.h"

// Additional
#include "OpenGL.h"

R_RenderBuffer* R_RenderBuffer::createRenderBuffer(uint32 width, uint32 height, R_TextureFormats::List format, bool depth, uint32 numColBufs, uint32 samples)
{
	if ((format == R_TextureFormats::RGBA16F || format == R_TextureFormats::RGBA32F) && !GetSettingsGroup<CGroupRenderCaps>().texFloat)
	{
		return 0;
	}

	if (numColBufs > R_RenderBuffer::MaxColorAttachmentCount) return 0;

	uint32 maxSamples = 0;
	if (GetSettingsGroup<CGroupRenderCaps>().rtMultisampling)
	{
		GLint value;
		glGetIntegerv(GL_MAX_SAMPLES, &value);
		maxSamples = (uint32)value;
	}
	if (samples > maxSamples)
	{
		samples = maxSamples;
		Log::Warn("GPU does not support desired multisampling quality for render target");
	}

	this->width = width;
	this->height = height;
	this->samples = samples;

	// Create framebuffers
	glGenFramebuffers(1, &this->fbo);
	if (samples > 0)
	{
		glGenFramebuffers(1, &this->fboMS);
	}

	if (numColBufs > 0)
	{
		// Attach color buffers
		for (uint32 j = 0; j < numColBufs; ++j)
		{
			// Create a color texture
			R_Texture* texObj = m_RenderDevice->createTexture(R_TextureTypes::Tex2D, this->width, this->height, 1, format, false, false, false, false);
			assert1(texObj != 0);
			texObj->uploadTextureData(0, 0, 0x0);
			this->colTexs[j] = texObj;

			glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

			// Attach the texture
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, GL_TEXTURE_2D, texObj->glObj, 0);

			if (samples > 0)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, this->fboMS);

				// Create a multisampled renderbuffer
				glGenRenderbuffers(1, &this->colBufs[j]);
				glBindRenderbuffer(GL_RENDERBUFFER, this->colBufs[j]);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, this->samples, texObj->glFmt, this->width, this->height);

				// Attach the renderbuffer
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, GL_RENDERBUFFER, this->colBufs[j]);
			}
		}

		uint32 buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
		glDrawBuffers(numColBufs, buffers);

		if (samples > 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, this->fboMS);
			glDrawBuffers(numColBufs, buffers);
		}
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (samples > 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, this->fboMS);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
	}

	// Attach depth buffer
	if (depth)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

		// Create a depth texture
		R_Texture* texObj = m_RenderDevice->createTexture(R_TextureTypes::Tex2D, this->width, this->height, 1, R_TextureFormats::DEPTH, false, false, false, false);
		assert1(texObj != 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		texObj->uploadTextureData(0, 0, 0x0);
		this->depthTex = texObj;

		// Attach the texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texObj->glObj, 0);

		if (samples > 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, this->fboMS);

			// Create a multisampled renderbuffer
			glGenRenderbuffers(1, &this->depthBuf);
			glBindRenderbuffer(GL_RENDERBUFFER, this->depthBuf);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, this->samples, m_RenderDevice->m_DepthFormat, this->width, this->height);

			// Attach the renderbuffer
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthBuf);
		}
	}

	// Check if FBO is complete
	bool valid = true;
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	uint32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->m_DefaultFBO);
	if (status != GL_FRAMEBUFFER_COMPLETE) valid = false;

	if (samples > 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->fboMS);
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->m_DefaultFBO);
		if (status != GL_FRAMEBUFFER_COMPLETE) valid = false;
	}

	if (!valid)
	{
		destroyRenderBuffer();
		return 0;
	}

	return this;
}

void R_RenderBuffer::destroyRenderBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->m_DefaultFBO);

	if (this->depthTex != 0)
	{
		this->depthTex->destroyTexture();
	}
	this->depthTex = 0;

	if (this->depthBuf != 0)
	{
		glDeleteRenderbuffers(1, &this->depthBuf);
	}
	this->depthBuf = 0;

	for (uint32 i = 0; i < R_RenderBuffer::MaxColorAttachmentCount; ++i)
	{
		if (this->colTexs[i] != 0)
		{
			this->colTexs[i]->destroyTexture();
		}
		this->colTexs[i] = 0;

		if (this->colBufs[i] != 0)
		{
			glDeleteRenderbuffers(1, &this->colBufs[i]);
		}
		this->colBufs[i] = 0;
	}

	if (this->fbo != 0)
	{
		glDeleteFramebuffers(1, &this->fbo);
	}
	this->fbo = 0;

	if (this->fboMS != 0)
	{
		glDeleteFramebuffers(1, &this->fboMS);
	}
	this->fboMS = 0;
}

void R_RenderBuffer::getRenderBufferDimensions(int *width, int *height)
{
	*width = this->width;
	*height = this->height;
}

R_Texture* R_RenderBuffer::getRenderBufferTex(uint32 bufIndex)
{
	if (bufIndex < R_RenderBuffer::MaxColorAttachmentCount)
	{
		return this->colTexs[bufIndex];
	}
	else if (bufIndex == 32)
	{
		return this->depthTex;
	}
	else
	{
		return 0;
	}
}

void R_RenderBuffer::setRenderBuffer()
{
	// Resolve render buffer if necessary
	if (m_RenderDevice->_curRendBuf != 0)
	{
		m_RenderDevice->_curRendBuf->resolveRenderBuffer();
	}

	// Set new render buffer
	m_RenderDevice->_curRendBuf = this;

	// Unbind all m_DiffuseTextures to make sure that no FBO attachment is bound any more
	for (uint32 i = 0; i < 16; ++i)
	{
		m_RenderDevice->setTexture(i, 0, 0, 0);
	}

	m_RenderDevice->commitStates(PM_TEXTURES);

	glBindFramebuffer(GL_FRAMEBUFFER, this->fboMS != 0 ? this->fboMS : this->fbo);
	assert1(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	m_RenderDevice->_fbWidth = this->width;
	m_RenderDevice->_fbHeight = this->height;

	if (this->fboMS != 0)
	{
		glEnable(GL_MULTISAMPLE);
	}
	else
	{
		glDisable(GL_MULTISAMPLE);
	}
}

void R_RenderBuffer::resetRenderBuffer()
{
	// Resolve render buffer if necessary
	if (m_RenderDevice->_curRendBuf != 0)
	{
		m_RenderDevice->_curRendBuf->resolveRenderBuffer();
	}

	// Set new render buffer
	m_RenderDevice->_curRendBuf = nullptr;

	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->m_DefaultFBO);
	if (m_RenderDevice->m_DefaultFBO == 0)
	{
		if (m_RenderDevice->_doubleBuffered)
			glDrawBuffer(m_RenderDevice->_outputBufferIndex == 1 ? GL_BACK_RIGHT : GL_BACK_LEFT);
		else
			glDrawBuffer(m_RenderDevice->_outputBufferIndex == 1 ? GL_FRONT_RIGHT : GL_FRONT_LEFT);
	}

	m_RenderDevice->_fbWidth = m_RenderDevice->m_ViewportWidth + m_RenderDevice->m_ViewportX;
	m_RenderDevice->_fbHeight = m_RenderDevice->m_ViewportHeight + m_RenderDevice->m_ViewportY;

	if (m_RenderDevice->M_DefaultFBOMultisampled)
	{
		glEnable(GL_MULTISAMPLE);
	}
	else
	{
		glDisable(GL_MULTISAMPLE);
	}
}

bool R_RenderBuffer::getRenderBufferData(int bufIndex, int *width, int *height, int *compCount, void *dataBuffer, int bufferSize)
{
	int x, y, w, h;
	int format = GL_RGBA;
	int type = GL_FLOAT;
	m_RenderDevice->beginRendering();
	glPixelStorei(GL_PACK_ALIGNMENT, 4);

	fail1();

	if (this == 0)
	{
		if (bufIndex != 32 && bufIndex != 0) return false;
		if (width != 0x0) *width = m_RenderDevice->m_ViewportWidth;
		if (height != 0x0) *height = m_RenderDevice->m_ViewportHeight;

		x = m_RenderDevice->m_ViewportX; y = m_RenderDevice->m_ViewportY; w = m_RenderDevice->m_ViewportWidth; h = m_RenderDevice->m_ViewportHeight;

		glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->m_DefaultFBO);
		if (bufIndex != 32)
		{
			if (m_RenderDevice->_doubleBuffered)
				glReadBuffer(m_RenderDevice->_outputBufferIndex == 1 ? GL_BACK_RIGHT : GL_BACK_LEFT);
			else
				glReadBuffer(m_RenderDevice->_outputBufferIndex == 1 ? GL_FRONT_RIGHT : GL_FRONT_LEFT);
		}
	}
	else
	{
		resolveRenderBuffer();

		if (bufIndex == 32 && this->depthTex == 0) return false;
		if (bufIndex != 32)
		{
			if ((unsigned)bufIndex >= R_RenderBuffer::MaxColorAttachmentCount || this->colTexs[bufIndex] == 0)
			{
				return false;
			}
		}
		if (width != 0x0) *width = this->width;
		if (height != 0x0) *height = this->height;

		x = 0; y = 0; w = this->width; h = this->height;

		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
		if (bufIndex != 32)
		{
			glReadBuffer(GL_COLOR_ATTACHMENT0 + bufIndex);
		}
	}

	if (bufIndex == 32)
	{
		format = GL_DEPTH_COMPONENT;
		type = GL_FLOAT;
	}

	int comps = (bufIndex == 32 ? 1 : 4);
	if (compCount != 0x0) *compCount = comps;

	bool retVal = false;
	if (dataBuffer != 0x0 && bufferSize >= w * h * comps * (type == GL_FLOAT ? 4 : 1))
	{
		glFinish();
		glReadPixels(x, y, w, h, format, type, dataBuffer);
		retVal = true;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->m_DefaultFBO);

	return retVal;
}

//

void R_RenderBuffer::resolveRenderBuffer()
{
	if (this->fboMS == 0)
	{
		return;
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fboMS);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);

	bool depthResolved = false;
	for (uint32 i = 0; i < R_RenderBuffer::MaxColorAttachmentCount; ++i)
	{
		if (this->colBufs[i] != 0)
		{
			glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);

			int mask = GL_COLOR_BUFFER_BIT;
			if (!depthResolved && this->depthBuf != 0)
			{
				mask |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
				depthResolved = true;
			}
			glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, mask, GL_NEAREST);
		}
	}

	if (!depthResolved && this->depthBuf != 0)
	{
		glReadBuffer(GL_NONE);
		glDrawBuffer(GL_NONE);
		glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RenderDevice->m_DefaultFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RenderDevice->m_DefaultFBO);
}