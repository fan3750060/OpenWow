#include <stdafx.h>

// Include
#include "RenderDeviceOGL.h"
#include "TextureOGL.h"
#include "StructuredBufferOGL.h"

// General
#include "RenderTargetOGL.h"

// Additional
#include "OpenGL.h"

GLenum TranslateAttachmentPoint(RenderTarget::AttachmentPoint _attach)
{
	GLenum result = GL_COLOR_ATTACHMENT0;
	switch (_attach)
	{
	case RenderTarget::AttachmentPoint::Color0:
		result = GL_COLOR_ATTACHMENT0;
		break;

	case RenderTarget::AttachmentPoint::Color1:
		result = GL_COLOR_ATTACHMENT1;
		break;

	case RenderTarget::AttachmentPoint::Color2:
		result = GL_COLOR_ATTACHMENT2;
		break;

	case RenderTarget::AttachmentPoint::Color3:
		result = GL_COLOR_ATTACHMENT3;
		break;

	case RenderTarget::AttachmentPoint::Color4:
		result = GL_COLOR_ATTACHMENT4;
		break;

	case RenderTarget::AttachmentPoint::Color5:
		result = GL_COLOR_ATTACHMENT5;
		break;

	case RenderTarget::AttachmentPoint::Color6:
		result = GL_COLOR_ATTACHMENT6;
		break;

	case RenderTarget::AttachmentPoint::Color7:
		result = GL_COLOR_ATTACHMENT7;
		break;

	case RenderTarget::AttachmentPoint::Depth:
		result = GL_DEPTH_ATTACHMENT;
		break;

	case RenderTarget::AttachmentPoint::DepthStencil:
		result = GL_DEPTH_ATTACHMENT;
		break;

	default:
		Log::Error("Unknown AttachmentPoint.");
	}

	return result;
}

RenderTargetOGL::RenderTargetOGL(RenderDeviceOGL* _device)
	: m_RenderDevice(_device)
	, m_Width(0)
	, m_Height(0)
	, m_bCheckValidity(false)
{
	m_Textures.resize((size_t)RenderTarget::AttachmentPoint::NumAttachmentPoints + 1);
	m_StructuredBuffers.resize(8);

	glGenFramebuffers(1, &m_FBGLObj);

	glDisable(GL_MULTISAMPLE);
}

RenderTargetOGL::~RenderTargetOGL()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->GetDefaultRB());

	if (m_FBGLObj != 0)
	{
		glDeleteFramebuffers(1, &m_FBGLObj);
	}
	m_FBGLObj = 0;
}

void RenderTargetOGL::AttachTexture(AttachmentPoint attachment, std::shared_ptr<Texture> texture)
{
	std::shared_ptr<TextureOGL> textureDX11 = std::dynamic_pointer_cast<TextureOGL>(texture);
	m_Textures[(uint8_t)attachment] = textureDX11;

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBGLObj);
	glFramebufferTexture2D(GL_FRAMEBUFFER, TranslateAttachmentPoint(attachment), GL_TEXTURE_2D, textureDX11->GetGLObject(), 0);
	OGLCheckError();

	if (attachment >= AttachmentPoint::Color0 && attachment <= AttachmentPoint::Color7)
	{
		GLenum buffers[] = { TranslateAttachmentPoint(attachment) };
		glDrawBuffers(1, buffers);
		OGLCheckError();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Next time the render target is "bound", check that it is valid.
	m_bCheckValidity = true;
}

std::shared_ptr<Texture> RenderTargetOGL::GetTexture(AttachmentPoint attachment)
{
	return m_Textures[(uint8_t)attachment];
}


void RenderTargetOGL::Clear(AttachmentPoint attachment, ClearFlags clearFlags, cvec4 color, float depth, uint8_t stencil)
{
	std::shared_ptr<TextureOGL> texture = m_Textures[(uint8_t)attachment];
	if (texture)
	{
		texture->Clear(clearFlags, color, depth, stencil);
	}
}

void RenderTargetOGL::Clear(ClearFlags clearFlags, cvec4 color, float depth, uint8_t stencil)
{
	for (uint8_t i = 0; i < (uint8_t)AttachmentPoint::NumAttachmentPoints; ++i)
	{
		Clear((AttachmentPoint)i, clearFlags, color, depth, stencil);
	}
}

void RenderTargetOGL::GenerateMipMaps()
{
	for (auto texture : m_Textures)
	{
		if (texture)
		{
			texture->GenerateMipMaps();
		}
	}
}

void RenderTargetOGL::AttachStructuredBuffer(uint8_t slot, std::shared_ptr<StructuredBuffer> rwBuffer)
{
	std::shared_ptr<StructuredBufferOGL> rwbufferDX11 = std::dynamic_pointer_cast<StructuredBufferOGL>(rwBuffer);
	m_StructuredBuffers[slot] = rwbufferDX11;

	// Next time the render target is "bound", check that it is valid.
	m_bCheckValidity = true;
}

std::shared_ptr<StructuredBuffer> RenderTargetOGL::GetStructuredBuffer(uint8_t slot)
{
	if (slot < m_StructuredBuffers.size())
	{
		return m_StructuredBuffers[slot];
	}
	return std::shared_ptr<StructuredBuffer>();
}


void RenderTargetOGL::Resize(uint16_t width, uint16_t height)
{
	if (m_Width != width || m_Height != height)
	{
		m_Width = glm::max<uint16_t>(width, 1);
		m_Height = glm::max<uint16_t>(height, 1);
		// Resize the attached textures.
		for (auto texture : m_Textures)
		{
			if (texture)
			{
				texture->Resize(m_Width, m_Height);
			}
		}
	}
}

void RenderTargetOGL::Bind()
{
	if (m_bCheckValidity)
	{
		if (!IsValid())
		{
			Log::Error("Invalid render target.");
		}
		m_bCheckValidity = false;
	}

	for (uint8_t i = 0; i < 8; i++)
	{
		std::shared_ptr<TextureOGL> texture = m_Textures[i];
		if (texture)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texture->GetGLObject());
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBGLObj);
	assert1(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void RenderTargetOGL::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->GetDefaultRB());

	if (m_RenderDevice->GetDefaultRB() == 0)
	{
		if (m_RenderDevice->IsDoubleBuffered())
			glDrawBuffer(GL_BACK_LEFT);
		else
			glDrawBuffer(GL_FRONT_LEFT);
	}
}

bool RenderTargetOGL::IsValid() const
{
	for (uint8 i = (uint8)AttachmentPoint::Color0; i <= (uint8)AttachmentPoint::Color7; i++)
	{
		std::shared_ptr<TextureOGL> texture = m_Textures[i];
		if (texture)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texture->GetGLObject());
		}
	}


	// Check if FBO is complete
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBGLObj);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	//glBindFramebuffer(GL_FRAMEBUFFER, m_RenderDevice->GetDefaultRB());

	switch (status) 
	{
	case GL_FRAMEBUFFER_COMPLETE:
		return true;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		Log::Error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		Log::Error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		Log::Error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		Log::Error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		Log::Error("GL_FRAMEBUFFER_UNSUPPORTED");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		Log::Error("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
		break;
	case GL_FRAMEBUFFER_UNDEFINED:
		Log::Error("GL_FRAMEBUFFER_UNDEFINED");
		break;

	default:
		Log::Error("GL_FRAMEBUFFER_UNKNOWN!!!");
		break;
	}
	return false;
}


