#include "stdafx.h"

// General
#include "R_Texture.h"

// Additional
#include "OpenGL.h"

static const uint32 textureTypes[3] = { GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP };

uint32 R_Texture::calcTextureSize(R_TextureFormats::List _format, int _width, int _height, int _depth)
{
	switch (_format)
	{
	case R_TextureFormats::RGBA8:
		return _width * _height * _depth * 4;
	case R_TextureFormats::DXT1:
		return std::max(_width / 4, 1) * std::max(_height / 4, 1) * _depth * 8;
	case R_TextureFormats::DXT3:
		return std::max(_width / 4, 1) * std::max(_height / 4, 1) * _depth * 16;
	case R_TextureFormats::DXT5:
		return std::max(_width / 4, 1) * std::max(_height / 4, 1) * _depth * 16;
	case R_TextureFormats::RGBA16F:
		return _width * _height * _depth * 8;
	case R_TextureFormats::RGBA32F:
		return _width * _height * _depth * 16;
	default:
		return 0;
	}
}

//--

R_Texture::R_Texture(cstring _name, RenderDevice* _renderDevice) :
	m_Name(_name),
	m_GLObj(0),
	m_GLFmt(0),
	m_Type(0),
	m_Format(R_TextureFormats::Unknown),
	m_Width(0),
	m_Height(0),
	m_Depth(0),
	m_MemSize(0),
	m_SamplerState(0),
	m_IssRGB(false),
	m_HasMips(false),
	m_GenMips(false),
	m_RenderDevice(_renderDevice)
{}

R_Texture::~R_Texture()
{
	if (m_GLObj)
	{
		glDeleteTextures(1, &m_GLObj);
	}

	m_RenderDevice->m_TextureMem -= m_MemSize;
}

R_Texture* R_Texture::createTexture(R_TextureTypes::List _type, int _width, int _height, int _depth, R_TextureFormats::List _format, bool _hasMips, bool _genMips, bool _compress, bool _sRGB)
{
	assert1(_depth > 0);

	if (!GetSettingsGroup<CGroupRenderCaps>().texNPOT)
	{
		// Check if texture is NPOT
		if ((_width & (_width - 1)) != 0 || (_height & (_height - 1)) != 0)
		{
			Log::Warn("R_Texture has non-power-of-two dimensions although NPOT is not supported by GPU");
		}
	}

	m_Type = textureTypes[_type];
	m_Format = _format;
	m_Width = _width;
	m_Height = _height;
	m_Depth = _depth;
	m_IssRGB = _sRGB /*&& _Config.sRGBLinearization*/; // FIXME
	m_GenMips = _genMips;
	m_HasMips = _hasMips;

	switch (_format)
	{
	case R_TextureFormats::RGBA8:
		m_GLFmt = m_IssRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
		break;
	case R_TextureFormats::DXT1:
		m_GLFmt = m_IssRGB ? GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT : GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case R_TextureFormats::DXT3:
		m_GLFmt = m_IssRGB ? GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT : GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case R_TextureFormats::DXT5:
		m_GLFmt = m_IssRGB ? GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT : GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	case R_TextureFormats::RGBA16F:
		m_GLFmt = GL_RGBA16F;
		break;
	case R_TextureFormats::RGBA32F:
		m_GLFmt = GL_RGBA32F;
		break;
	case R_TextureFormats::DEPTH:
		m_GLFmt = m_RenderDevice->m_DepthFormat;
		break;
	default:
		fail1();
		break;
	};

	glGenTextures(1, &m_GLObj);
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(m_Type, m_GLObj);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	m_SamplerState = 0;
	m_RenderDevice->applySamplerState(this);

	glBindTexture(m_Type, 0);
	if (m_RenderDevice->m_State.m_TextureSlot[15].m_Texture)
	{
		glBindTexture(m_RenderDevice->m_State.m_TextureSlot[15].m_Texture->m_Type, m_RenderDevice->m_State.m_TextureSlot[15].m_Texture->m_GLObj);
	}

	// Calculate memory requirements
	m_MemSize = calcTextureSize(_format, _width, _height, _depth);
	if (_hasMips || _genMips)
	{
		m_MemSize += ftoi_r(m_MemSize * 1.0f / 3.0f);
	}
	if (_type == R_TextureTypes::TexCube)
	{
		m_MemSize *= 6;
	}
	m_RenderDevice->m_TextureMem += m_MemSize;

	return this;
}

void R_Texture::uploadTextureData(int _slice, int _mipLevel, const void* _pixels)
{
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(m_Type, m_GLObj);

	int inputFormat = GL_RGBA, inputType = GL_UNSIGNED_BYTE;
	bool compressed = (m_Format == R_TextureFormats::DXT1) || (m_Format == R_TextureFormats::DXT3) || (m_Format == R_TextureFormats::DXT5);

	switch (m_Format)
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
	int width = std::max(m_Width >> _mipLevel, 1), height = std::max(m_Height >> _mipLevel, 1);

	if (m_Type == textureTypes[R_TextureTypes::Tex2D] || m_Type == textureTypes[R_TextureTypes::TexCube])
	{
		int target = (m_Type == textureTypes[R_TextureTypes::Tex2D]) ? GL_TEXTURE_2D : (GL_TEXTURE_CUBE_MAP_POSITIVE_X + _slice);

		if (compressed)
			glCompressedTexImage2D(target, _mipLevel, m_GLFmt, width, height, 0, calcTextureSize(m_Format, width, height, 1), _pixels);
		else
			glTexImage2D(target, _mipLevel, m_GLFmt, width, height, 0, inputFormat, inputType, _pixels);
	}
	else if (m_Type == textureTypes[R_TextureTypes::Tex3D])
	{
		int depth = std::max(m_Depth >> _mipLevel, 1);

		if (compressed)
			glCompressedTexImage3D(GL_TEXTURE_3D, _mipLevel, m_GLFmt, width, height, depth, 0, calcTextureSize(m_Format, width, height, depth), _pixels);
		else
			glTexImage3D(GL_TEXTURE_3D, _mipLevel, m_GLFmt, width, height, depth, 0, inputFormat, inputType, _pixels);
	}

	if (m_GenMips && (m_Type != GL_TEXTURE_CUBE_MAP || _slice == 5))
	{
		glGenerateMipmap(m_Type);
	}

	glBindTexture(m_Type, 0);
	if (m_RenderDevice->m_State.m_TextureSlot[15].m_Texture)
	{
		glBindTexture(m_RenderDevice->m_State.m_TextureSlot[15].m_Texture->m_Type, m_RenderDevice->m_State.m_TextureSlot[15].m_Texture->m_GLObj);
	}
}

bool R_Texture::getTextureData(int _slice, int _mipLevel, void* _buffer)
{
	int target = (m_Type == textureTypes[R_TextureTypes::TexCube] ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D);
	if (target == GL_TEXTURE_CUBE_MAP)
	{
		target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + _slice;
	}

	int fmt, type, compressed = 0;
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(m_Type, m_GLObj);

	switch (m_Format)
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
		glGetCompressedTexImage(target, _mipLevel, _buffer);
	}
	else
	{
		glGetTexImage(target, _mipLevel, fmt, type, _buffer);
	}

	glBindTexture(m_Type, 0);
	if (m_RenderDevice->m_State.m_TextureSlot[15].m_Texture)
	{
		glBindTexture(m_RenderDevice->m_State.m_TextureSlot[15].m_Texture->m_Type, m_RenderDevice->m_State.m_TextureSlot[15].m_Texture->m_GLObj);
	}

	return true;
}

void R_Texture::bindImageToTexture(void* _eglImage)
{
	if (!GetSettingsGroup<CGroupOpenGL>().OES_EGL_image)
	{
		Log::Error("OES_egl_image not supported");
	}
	else
	{
		glActiveTexture(GL_TEXTURE15);
		glBindTexture(m_Type, m_GLObj);
		glEGLImageTargetTexture2DOES(m_Type, _eglImage);
		m_RenderDevice->checkError();
		glBindTexture(m_Type, 0);
		if (m_RenderDevice->m_State.m_TextureSlot[15].m_Texture)
		{
			glBindTexture(m_RenderDevice->m_State.m_TextureSlot[15].m_Texture->m_Type, m_RenderDevice->m_State.m_TextureSlot[15].m_Texture->m_GLObj);
		}
	}
}