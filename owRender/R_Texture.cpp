#include "stdafx.h"

// General
#include "R_Texture.h"

// Additional
#include "OpenGL.h"

static const uint32 textureTypes[3] = { GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP };

uint32 R_Texture::calcTextureSize(R_TextureFormats::List format, int width, int height, int depth)
{
	switch (format)
	{
	case R_TextureFormats::RGBA8:
		return width * height * depth * 4;
	case R_TextureFormats::DXT1:
		return std::max(width / 4, 1) * std::max(height / 4, 1) * depth * 8;
	case R_TextureFormats::DXT3:
		return std::max(width / 4, 1) * std::max(height / 4, 1) * depth * 16;
	case R_TextureFormats::DXT5:
		return std::max(width / 4, 1) * std::max(height / 4, 1) * depth * 16;
	case R_TextureFormats::RGBA16F:
		return width * height * depth * 8;
	case R_TextureFormats::RGBA32F:
		return width * height * depth * 16;
	default:
		return 0;
	}
}

//--

R_Texture* R_Texture::createTexture(R_TextureTypes::List type, int width, int height, int depth, R_TextureFormats::List format, bool hasMips, bool genMips, bool compress, bool sRGB)
{
	assert1(depth > 0);

	if (!GetSettingsGroup<CGroupRenderCaps>().texNPOT)
	{
		// Check if texture is NPOT
		if ((width & (width - 1)) != 0 || (height & (height - 1)) != 0)
			Log::Warn("R_Texture has non-power-of-two dimensions although NPOT is not supported by GPU");
	}

	this->type = textureTypes[type];
	this->format = format;
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->sRGB = sRGB /*&& _Config.sRGBLinearization*/; // FIXME
	this->genMips = genMips;
	this->hasMips = hasMips;

	switch (format)
	{
	case R_TextureFormats::RGBA8:
		this->glFmt = this->sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
		break;
	case R_TextureFormats::DXT1:
		this->glFmt = this->sRGB ? GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT : GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case R_TextureFormats::DXT3:
		this->glFmt = this->sRGB ? GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT : GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case R_TextureFormats::DXT5:
		this->glFmt = this->sRGB ? GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT : GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	case R_TextureFormats::RGBA16F:
		this->glFmt = GL_RGBA16F;
		break;
	case R_TextureFormats::RGBA32F:
		this->glFmt = GL_RGBA32F;
		break;
	case R_TextureFormats::DEPTH:
		this->glFmt = m_RenderDevice->m_DepthFormat;
		break;
	default:
		fail1();
		break;
	};

	glGenTextures(1, &this->glObj);
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(this->type, this->glObj);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	this->samplerState = 0;
	m_RenderDevice->applySamplerState(this);

	glBindTexture(this->type, 0);
	if (m_RenderDevice->_texSlots[15].texObj)
		glBindTexture(m_RenderDevice->_texSlots[15].texObj->type, m_RenderDevice->_texSlots[15].texObj->glObj);

	// Calculate memory requirements
	this->memSize = calcTextureSize(format, width, height, depth);
	if (hasMips || genMips) this->memSize += ftoi_r(this->memSize * 1.0f / 3.0f);
	if (type == R_TextureTypes::TexCube) this->memSize *= 6;
	m_RenderDevice->_textureMem += this->memSize;

	return this;
}

void R_Texture::uploadTextureData(int slice, int mipLevel, const void *pixels)
{
	R_TextureFormats::List format = this->format;

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(this->type, this->glObj);

	int inputFormat = GL_RGBA, inputType = GL_UNSIGNED_BYTE;
	bool compressed = (format == R_TextureFormats::DXT1) || (format == R_TextureFormats::DXT3) || (format == R_TextureFormats::DXT5);

	switch (format)
	{
	case R_TextureFormats::RGBA16F:
		inputFormat = GL_RGBA;
		inputType = GL_FLOAT;
		break;

	case R_TextureFormats::RGBA32F:
		inputFormat = GL_RGBA;
		inputType = GL_FLOAT;
		break;

	case R_TextureFormats::DEPTH:
		inputFormat = GL_DEPTH_COMPONENT;
		inputType = GL_FLOAT;
		break;

	};

	// Calculate size of next mipmap using "floor" convention
	int width = std::max(this->width >> mipLevel, 1), height = std::max(this->height >> mipLevel, 1);

	if (this->type == textureTypes[R_TextureTypes::Tex2D] || this->type == textureTypes[R_TextureTypes::TexCube])
	{
		int target = (this->type == textureTypes[R_TextureTypes::Tex2D]) ? GL_TEXTURE_2D : (GL_TEXTURE_CUBE_MAP_POSITIVE_X + slice);

		if (compressed)
			glCompressedTexImage2D(target, mipLevel, this->glFmt, width, height, 0, calcTextureSize(format, width, height, 1), pixels);
		else
			glTexImage2D(target, mipLevel, this->glFmt, width, height, 0, inputFormat, inputType, pixels);
	}
	else if (this->type == textureTypes[R_TextureTypes::Tex3D])
	{
		int depth = std::max(this->depth >> mipLevel, 1);

		if (compressed)
			glCompressedTexImage3D(GL_TEXTURE_3D, mipLevel, this->glFmt, width, height, depth, 0, calcTextureSize(format, width, height, depth), pixels);
		else
			glTexImage3D(GL_TEXTURE_3D, mipLevel, this->glFmt, width, height, depth, 0, inputFormat, inputType, pixels);
	}

	if (this->genMips && (this->type != GL_TEXTURE_CUBE_MAP || slice == 5))
	{
		glGenerateMipmap(this->type);
	}

	glBindTexture(this->type, 0);
	if (m_RenderDevice->_texSlots[15].texObj)
	{
		glBindTexture(m_RenderDevice->_texSlots[15].texObj->type, m_RenderDevice->_texSlots[15].texObj->glObj);
	}
}

void R_Texture::destroyTexture()
{
	if (glObj)
	{
		glDeleteTextures(1, &glObj);
	}

	m_RenderDevice->_textureMem -= memSize;
}

bool R_Texture::getTextureData(int slice, int mipLevel, void *buffer)
{
	int target = type == textureTypes[R_TextureTypes::TexCube] ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
	if (target == GL_TEXTURE_CUBE_MAP) target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + slice;

	int fmt, type, compressed = 0;
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(type, glObj);

	switch (format)
	{
	case R_TextureFormats::RGBA8:
		fmt = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		break;
	case R_TextureFormats::DXT1:
	case R_TextureFormats::DXT3:
	case R_TextureFormats::DXT5:
		compressed = 1;
		break;
	case R_TextureFormats::RGBA16F:
	case R_TextureFormats::RGBA32F:
		fmt = GL_RGBA;
		type = GL_FLOAT;
		break;
	default:
		return false;
	};

	if (compressed)
	{
		glGetCompressedTexImage(target, mipLevel, buffer);
	}
	else
	{
		glGetTexImage(target, mipLevel, fmt, type, buffer);
	}

	glBindTexture(type, 0);
	if (m_RenderDevice->_texSlots[15].texObj)
	{
		glBindTexture(m_RenderDevice->_texSlots[15].texObj->type, m_RenderDevice->_texSlots[15].texObj->glObj);
	}

	return true;
}

void R_Texture::bindImageToTexture(void *eglImage)
{
	if (!GetSettingsGroup<CGroupOpenGL>().OES_EGL_image)
	{
		Log::Error("OES_egl_image not supported");
	}
	else
	{
		glActiveTexture(GL_TEXTURE15);
		glBindTexture(type, glObj);
		glEGLImageTargetTexture2DOES(type, eglImage);
		m_RenderDevice->checkError();
		glBindTexture(type, 0);
		if (m_RenderDevice->_texSlots[15].texObj)
		{
			glBindTexture(m_RenderDevice->_texSlots[15].texObj->type, m_RenderDevice->_texSlots[15].texObj->glObj);
		}
	}
}