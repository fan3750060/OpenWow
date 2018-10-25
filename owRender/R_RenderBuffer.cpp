#include "stdafx.h"

// General
#include "R_RenderBuffer.h"

// Additional
#include "OpenGL.h"

R_RenderBuffer::R_RenderBuffer(RenderDevice* _RenderDevice) :
	m_FBGLObj(0),
	m_FBMultiSampledGLObj(0),
	m_Width(0),
	m_Height(0),
	m_DepthTexture(0),
	m_DepthBufferGLObj(0),
	m_Samples(0),
	m_RenderDevice(_RenderDevice)
{
	for (uint32 i = 0; i < MaxColorAttachmentCount; ++i)
	{
		m_ColorsTextures[i] = nullptr;
		m_ColorsBuffersGLObj[i] = 0;
	}
}

R_RenderBuffer::~R_RenderBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->m_RBDefault);

	if (m_DepthBufferGLObj != 0)
	{
		glDeleteRenderbuffers(1, &m_DepthBufferGLObj);
	}
	m_DepthBufferGLObj = 0;

	for (uint32 i = 0; i < R_RenderBuffer::MaxColorAttachmentCount; ++i)
	{
		if (m_ColorsBuffersGLObj[i] != 0)
		{
			glDeleteRenderbuffers(1, &m_ColorsBuffersGLObj[i]);
		}
		m_ColorsBuffersGLObj[i] = 0;
	}

	if (m_FBGLObj != 0)
	{
		glDeleteFramebuffers(1, &m_FBGLObj);
	}
	m_FBGLObj = 0;

	if (m_FBMultiSampledGLObj != 0)
	{
		glDeleteFramebuffers(1, &m_FBMultiSampledGLObj);
	}
	m_FBMultiSampledGLObj = 0;
}

void R_RenderBuffer::createRenderBuffer(uint32 _width, uint32 _height, R_TextureFormats::List _format, bool _depth, uint32 _numColBufs, uint32 _samples)
{
	if ((_format == R_TextureFormats::RGBA16F || _format == R_TextureFormats::RGBA32F) && !GetSettingsGroup<CGroupRenderCaps>().texFloat)
	{
		return;
	}

	if (_numColBufs > R_RenderBuffer::MaxColorAttachmentCount)
	{
		return;
	}

	// Get samples
	uint32 maxSamples = 0;
	if (GetSettingsGroup<CGroupRenderCaps>().rtMultisampling)
	{
		GLint value;
		glGetIntegerv(GL_MAX_SAMPLES, &value);
		maxSamples = (uint32)value;
	}
	if (_samples > maxSamples)
	{
		_samples = maxSamples;
		Log::Warn("GPU does not support desired multisampling quality for render target");
	}

	m_Width = _width;
	m_Height = _height;
	m_Samples = _samples;

	// Create framebuffers
	glGenFramebuffers(1, &m_FBGLObj);
	if (_samples > 0)
	{
		glGenFramebuffers(1, &m_FBMultiSampledGLObj);
	}

	if (_numColBufs > 0)
	{
		// Attach color buffers
		for (uint32 j = 0; j < _numColBufs; ++j)
		{
			// Create a color texture
			std::shared_ptr<Texture> texObj = m_RenderDevice->createTexture(R_TextureTypes::Tex2D, m_Width, m_Height, 1, _format, false, false, false, false);
			_ASSERT(texObj != nullptr);
			texObj->uploadTextureData(0, 0, 0x0);
			m_ColorsTextures[j] = texObj;

			glBindFramebuffer(GL_FRAMEBUFFER, m_FBGLObj);

			// Attach the texture
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, GL_TEXTURE_2D, texObj->m_GLObj, 0);

			if (_samples > 0)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_FBMultiSampledGLObj);

				// Create a multisampled renderbuffer
				glGenRenderbuffers(1, &m_ColorsBuffersGLObj[j]);
				glBindRenderbuffer(GL_RENDERBUFFER, m_ColorsBuffersGLObj[j]);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Samples, texObj->m_GLFmt, m_Width, m_Height);

				// Attach the renderbuffer
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, GL_RENDERBUFFER, m_ColorsBuffersGLObj[j]);
			}
		}

		GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBGLObj);
		glDrawBuffers(_numColBufs, buffers);

		if (_samples > 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBMultiSampledGLObj);
			glDrawBuffers(_numColBufs, buffers);
		}
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBGLObj);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (_samples > 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBMultiSampledGLObj);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
	}

	// Attach depth buffer
	if (_depth)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBGLObj);

		// Create a depth texture
		std::shared_ptr<Texture> texObj = m_RenderDevice->createTexture(R_TextureTypes::Tex2D, m_Width, m_Height, 1, R_TextureFormats::DEPTH, false, false, false, false);
		_ASSERT(texObj != nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		texObj->uploadTextureData(0, 0, 0x0);
		m_DepthTexture = texObj;

		// Attach the texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texObj->m_GLObj, 0);

		if (_samples > 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBMultiSampledGLObj);

			// Create a multisampled renderbuffer
			glGenRenderbuffers(1, &m_DepthBufferGLObj);
			glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferGLObj);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Samples, m_RenderDevice->m_DepthFormat, m_Width, m_Height);

			// Attach the renderbuffer
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufferGLObj);
		}
	}

	// Check if FBO is complete
	bool valid = true;
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBGLObj);
	uint32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->m_RBDefault);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		valid = false;
	}

	if (_samples > 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBMultiSampledGLObj);
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->m_RBDefault);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			valid = false;
		}
	}

	if (!valid)
	{
		delete this;
		return;
	}

	return;
}

void R_RenderBuffer::getRenderBufferDimensions(int *width, int *height)
{
	*width = m_Width;
	*height = m_Height;
}

std::shared_ptr<Texture> R_RenderBuffer::getRenderBufferTex(uint32 _bufIndex)
{
	if (_bufIndex < R_RenderBuffer::MaxColorAttachmentCount)
	{
		return m_ColorsTextures[_bufIndex];
	}
	else if (_bufIndex == 32)
	{
		return m_DepthTexture;
	}

	return nullptr;
}

void R_RenderBuffer::setRenderBuffer()
{
	// Resolve render buffer if necessary
	if (m_RenderDevice->m_RBCurrent != nullptr)
	{
		m_RenderDevice->m_RBCurrent->resolveRenderBuffer();
	}

	// Set new render buffer
	m_RenderDevice->m_RBCurrent = this;

	// Unbind all m_DiffuseTextures to make sure that no FBO attachment is bound any more
	for (uint32 i = 0; i < 16; ++i)
	{
		m_RenderDevice->setTexture(i, 0, 0, 0);
	}

	m_RenderDevice->commitStates(nullptr, PM_TEXTURES);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBMultiSampledGLObj != 0 ? m_FBMultiSampledGLObj : m_FBGLObj);
	_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	m_RenderDevice->m_RBWidth = m_Width;
	m_RenderDevice->m_RBHeight = m_Height;

	if (m_FBMultiSampledGLObj != 0)
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
	if (m_RenderDevice->m_RBCurrent != nullptr)
	{
		m_RenderDevice->m_RBCurrent->resolveRenderBuffer();
	}

	// Set new render buffer
	m_RenderDevice->m_RBCurrent = nullptr;

	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->m_RBDefault);
	if (m_RenderDevice->m_RBDefault == 0)
	{
		if (m_RenderDevice->_doubleBuffered)
			glDrawBuffer(m_RenderDevice->_outputBufferIndex == 1 ? GL_BACK_RIGHT : GL_BACK_LEFT);
		else
			glDrawBuffer(m_RenderDevice->_outputBufferIndex == 1 ? GL_FRONT_RIGHT : GL_FRONT_LEFT);
	}

	m_RenderDevice->m_RBWidth = m_RenderDevice->m_State.m_ViewportWidth + m_RenderDevice->m_State.m_ViewportX;
	m_RenderDevice->m_RBHeight = m_RenderDevice->m_State.m_ViewportHeight + m_RenderDevice->m_State.m_ViewportY;

	if (m_RenderDevice->m_IsDefaultFBOMultisampled)
	{
		glEnable(GL_MULTISAMPLE);
	}
	else
	{
		glDisable(GL_MULTISAMPLE);
	}
}

bool R_RenderBuffer::getRenderBufferData(int _bufIndex, int* _width, int* _height, int* _compCount, void* _dataBuffer, int _bufferSize)
{
	int x, y, w, h;
	int format = GL_RGBA;
	int type = GL_FLOAT;
	m_RenderDevice->beginRendering();
	glPixelStorei(GL_PACK_ALIGNMENT, 4);

	fail1();

	if (this == 0)
	{
		if (_bufIndex != 32 && _bufIndex != 0) return false;
		if (_width != 0x0) *_width = m_RenderDevice->m_State.m_ViewportWidth;
		if (_height != 0x0) *_height = m_RenderDevice->m_State.m_ViewportHeight;

		x = m_RenderDevice->m_State.m_ViewportX; y = m_RenderDevice->m_State.m_ViewportY; w = m_RenderDevice->m_State.m_ViewportWidth; h = m_RenderDevice->m_State.m_ViewportHeight;

		glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->m_RBDefault);
		if (_bufIndex != 32)
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

		if (_bufIndex == 32 && m_DepthTexture == nullptr)
		{
			return false;
		}

		if (_bufIndex != 32)
		{
			if ((unsigned)_bufIndex >= R_RenderBuffer::MaxColorAttachmentCount || m_ColorsTextures[_bufIndex] == nullptr)
			{
				return false;
			}
		}
		if (_width != 0x0) *_width = m_Width;
		if (_height != 0x0) *_height = m_Height;

		x = 0; y = 0; w = m_Width; h = m_Height;

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBGLObj);
		if (_bufIndex != 32)
		{
			glReadBuffer(GL_COLOR_ATTACHMENT0 + _bufIndex);
		}
	}

	if (_bufIndex == 32)
	{
		format = GL_DEPTH_COMPONENT;
		type = GL_FLOAT;
	}

	int comps = (_bufIndex == 32 ? 1 : 4);
	if (_compCount != 0x0) *_compCount = comps;

	bool retVal = false;
	if (_dataBuffer != 0x0 && _bufferSize >= w * h * comps * (type == GL_FLOAT ? 4 : 1))
	{
		glFinish();
		glReadPixels(x, y, w, h, format, type, _dataBuffer);
		retVal = true;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->m_RBDefault);

	return retVal;
}

//

void R_RenderBuffer::resolveRenderBuffer()
{
	if (m_FBMultiSampledGLObj == 0)
	{
		return;
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBMultiSampledGLObj);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBGLObj);

	bool depthResolved = false;
	for (uint32 i = 0; i < R_RenderBuffer::MaxColorAttachmentCount; ++i)
	{
		if (m_ColorsBuffersGLObj[i] != 0)
		{
			glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);

			int mask = GL_COLOR_BUFFER_BIT;
			if (!depthResolved && m_DepthBufferGLObj != 0)
			{
				mask |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
				depthResolved = true;
			}
			glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, mask, GL_NEAREST);
		}
	}

	if (!depthResolved && m_DepthBufferGLObj != 0)
	{
		glReadBuffer(GL_NONE);
		glDrawBuffer(GL_NONE);
		glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RenderDevice->m_RBDefault);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RenderDevice->m_RBDefault);
}