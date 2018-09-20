#include <stdafx.h>

// General
#include "TextureOGL.h"

// Additional
#include "OpenGL.h"

#include __PACK_BEGIN
struct BLPHeader
{
	uint8 magic[4];
	uint32 type;

	uint8 compression;  // Compression: 1 for uncompressed, 2 for DXTC, 3 (cataclysm) for plain A8R8G8B8 m_DiffuseTextures
	uint8 alpha_depth;  // Alpha channel bit depth: 0, 1, 4 or 8
	uint8 alpha_type;   // 0, 1, 7, or 8
	uint8 has_mips;     // 0 = no mips, 1 = has mips

	uint32 width;
	uint32 height;
	uint32 mipOffsets[16];
	uint32 mipSizes[16];
};
#include __PACK_END

TextureOGL::TextureOGL()
	: m_TextureWidth(0)
	, m_TextureHeight(0)
	, m_NumSlices(0)
	, m_CPUAccess(CPUAccess::None)
	, m_bDynamic(false)
	, m_bUAV(false)
	, m_bGenerateMipmaps(false)
	, m_BPP(0)
	, m_Pitch(0)
	, m_bIsTransparent(false)
	, m_bIsDirty(false)
{

}

// 2D Texture
TextureOGL::TextureOGL(uint16_t width, uint16_t height, uint16_t slices, const TextureFormat& format, CPUAccess cpuAccess, bool bUAV)
	: m_TextureWidth(0)
	, m_TextureHeight(0)
	, m_BPP(0)
	, m_TextureFormat(format)
	, m_CPUAccess(cpuAccess)
	, m_bGenerateMipmaps(false)
	, m_bIsTransparent(true)
	, m_bIsDirty(false)
{
	m_NumSlices = glm::max<uint16_t>(slices, 1);

	m_TextureDimension = Dimension::Texture2D;
	if (m_NumSlices > 1)
	{
		m_TextureDimension = Dimension::Texture2DArray;
	}

	glGenTextures(1, &m_GLObj);
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, m_GLObj);


	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Sampler state

	glBindTexture(GL_TEXTURE_2D, 0);

}

// CUBE Texture
TextureOGL::TextureOGL(uint16_t size, uint16_t count, const TextureFormat& format, CPUAccess cpuAccess, bool bUAV)
{
	m_TextureDimension = Texture::Dimension::TextureCube;

	m_TextureWidth = m_TextureHeight = size;

	glGenTextures(1, &m_GLObj);
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_GLObj);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Sampler state

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

TextureOGL::~TextureOGL()
{}

struct R_TextureFormats
{
	enum List
	{
		Unknown,
		RGBA8,
		DXT1,
		DXT3,
		DXT5,
		RGBA16F,
		RGBA32F,
		DEPTH,
		R32,
		RG32
	};
};

bool TextureOGL::LoadTexture2D(cstring fileName)
{
	return true;
}

bool TextureOGL::LoadTextureCube(cstring fileName)
{
	return false;
}

void TextureOGL::GenerateMipMaps()
{
	if (m_bGenerateMipmaps && (m_TextureType != GL_TEXTURE_CUBE_MAP))
	{
		glGenerateMipmap(m_TextureType);
	}
}

std::shared_ptr<Texture> TextureOGL::GetFace(CubeFace face) const
{
	return std::static_pointer_cast<Texture>(std::const_pointer_cast<TextureOGL>(shared_from_this()));
}

std::shared_ptr<Texture> TextureOGL::GetSlice(uint32 slice) const
{
	return std::static_pointer_cast<Texture>(std::const_pointer_cast<TextureOGL>(shared_from_this()));
}

uint16_t TextureOGL::GetWidth() const
{
	return m_TextureWidth;
}

uint16_t TextureOGL::GetHeight() const
{
	return m_TextureHeight;
}

uint16_t TextureOGL::GetDepth() const
{
	return m_NumSlices;
}

uint8_t TextureOGL::GetBPP() const
{
	return m_BPP;
}

bool TextureOGL::IsTransparent() const
{
	return m_bIsTransparent;
}

void TextureOGL::Resize2D(uint16_t width, uint16_t height)
{
	throw std::exception("Not implemented!");
}

void TextureOGL::ResizeCube(uint16_t size)
{
	throw std::exception("Not implemented!");
}

void TextureOGL::Resize(uint16_t width, uint16_t height, uint16_t depth)
{
	switch (m_TextureDimension)
	{
	case Dimension::Texture2D:
	case Dimension::Texture2DArray:
		Resize2D(width, height);
		break;
	case Texture::Dimension::TextureCube:
		ResizeCube(width);
		break;
	default:
		Log::Error("Unknown texture dimension.");
		break;
	}

	return;
}

uint32 TextureOGL::GetGLObject()
{
	return m_GLObj;
}

void TextureOGL::Plot(glm::ivec2 coord, const uint8_t* pixel, size_t size)
{
	assert(m_BPP > 0 && m_BPP % 8 == 0);
	assert(coord.s < m_TextureWidth && coord.t < m_TextureHeight && size == (m_BPP / 8));

	uint8_t bytesPerPixel = (m_BPP / 8);
	uint32_t stride = m_TextureWidth * bytesPerPixel;
	uint32_t index = (coord.s * bytesPerPixel) + (coord.t * stride);

	for (uint32 i = 0; i < size; ++i)
	{
		m_Buffer[index + i] = *(pixel + i);
	}

	m_bIsDirty = true;
}

void TextureOGL::FetchPixel(glm::ivec2 coord, uint8_t*& pixel, size_t size)
{
	assert(m_BPP > 0 && m_BPP % 8 == 0);
	assert(coord.s < m_TextureWidth && coord.t < m_TextureHeight && size == (m_BPP / 8));

	uint8_t bytesPerPixel = (m_BPP / 8);
	uint32_t stride = m_TextureWidth * bytesPerPixel;
	uint32_t index = (coord.s * bytesPerPixel) + (coord.t * stride);
	pixel = &m_Buffer[index];
}

void TextureOGL::Copy(std::shared_ptr<Texture> other)
{
	std::shared_ptr<TextureOGL> srcTexture = std::dynamic_pointer_cast<TextureOGL>(other);

	if (srcTexture && srcTexture.get() != this)
	{
		if (m_TextureDimension == srcTexture->m_TextureDimension &&
			m_TextureWidth == srcTexture->m_TextureWidth &&
			m_TextureHeight == srcTexture->m_TextureHeight
		)
		{
			switch (m_TextureDimension)
			{
			case Texture::Dimension::Texture2D:
			case Texture::Dimension::Texture2DArray:
				throw std::exception("Not implemented!");
				//m_pDeviceContext->CopyResource(m_pTexture2D, srcTexture->m_pTexture2D);
				break;
			case Texture::Dimension::TextureCube:
				throw std::exception("Not implemented!");
				//m_pDeviceContext->CopyResource(m_pTexture3D, srcTexture->m_pTexture3D);
				break;
			}
		}
		else
		{
			Log::Error("Incompatible source texture.");
		}
	}

	/*if (((int)m_CPUAccess & (int)CPUAccess::Read) != 0 && m_pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		// Copy the texture data from the texture resource
		if (FAILED(m_pDeviceContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &mappedResource)))
		{
			Log::Error("Failed to map texture resource for reading.");
		}

		memcpy_s(m_Buffer.data(), m_Buffer.size(), mappedResource.pData, m_Buffer.size());

		m_pDeviceContext->Unmap(m_pTexture2D, 0);
	}*/
}

void TextureOGL::Clear(ClearFlags clearFlags, cvec4 color, float depth, uint8_t stencil)
{
	/*if (m_pRenderTargetView && ((int)clearFlags & (int)ClearFlags::Color) != 0)
	{
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, glm::value_ptr(color));
	}

	{
		UINT flags = 0;
		flags |= ((int)clearFlags & (int)ClearFlags::Depth) != 0 ? D3D11_CLEAR_DEPTH : 0;
		flags |= ((int)clearFlags & (int)ClearFlags::Stencil) != 0 ? D3D11_CLEAR_STENCIL : 0;
		if (m_pDepthStencilView && flags > 0)
		{
			m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, flags, depth, stencil);
		}
	}*/
}

void TextureOGL::Bind(uint32_t ID, Shader::ShaderType shaderType, ShaderParameter::Type parameterType)
{
	if (m_bIsDirty)
	{
		/*if (m_bDynamic && m_pTexture2D)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			// Copy the texture data to the texture resource
			HRESULT hr = m_pDeviceContext->Map(m_pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(hr))
			{
				Log::Error("Failed to map texture resource for writing.");
			}

			memcpy_s(mappedResource.pData, m_Buffer.size(), m_Buffer.data(), m_Buffer.size());

			m_pDeviceContext->Unmap(m_pTexture2D, 0);

			if (m_bGenerateMipmaps)
			{
				m_pDeviceContext->GenerateMips(m_pShaderResourceView);
			}
		}*/
		m_bIsDirty = false;
	}

	/*ID3D11ShaderResourceView* srv[] = { m_pShaderResourceView };
	ID3D11UnorderedAccessView* uav[] = { m_pUnorderedAccessView };

	if (parameterType == ShaderParameter::Type::Texture && m_pShaderResourceView)
	{
		switch (shaderType)
		{
		case Shader::VertexShader:
			m_pDeviceContext->VSSetShaderResources(ID, 1, srv);
			break;
		case Shader::TessellationControlShader:
			m_pDeviceContext->HSSetShaderResources(ID, 1, srv);
			break;
		case Shader::TessellationEvaluationShader:
			m_pDeviceContext->DSSetShaderResources(ID, 1, srv);
			break;
		case Shader::GeometryShader:
			m_pDeviceContext->GSSetShaderResources(ID, 1, srv);
			break;
		case Shader::PixelShader:
			m_pDeviceContext->PSSetShaderResources(ID, 1, srv);
			break;
		case Shader::ComputeShader:
			m_pDeviceContext->CSSetShaderResources(ID, 1, srv);
			break;
		}
	}
	else if (parameterType == ShaderParameter::Type::RWTexture && m_pUnorderedAccessView)
	{
		switch (shaderType)
		{
		case Shader::ComputeShader:
			m_pDeviceContext->CSSetUnorderedAccessViews(ID, 1, uav, nullptr);
			break;
		}
	}*/

}
void TextureOGL::UnBind(uint32_t ID, Shader::ShaderType shaderType, ShaderParameter::Type parameterType)
{
	/*ID3D11ShaderResourceView* srv[] = { nullptr };
	ID3D11UnorderedAccessView* uav[] = { nullptr };

	if (parameterType == ShaderParameter::Type::Texture)
	{
		switch (shaderType)
		{
		case Shader::VertexShader:
			m_pDeviceContext->VSSetShaderResources(ID, 1, srv);
			break;
		case Shader::TessellationControlShader:
			m_pDeviceContext->HSSetShaderResources(ID, 1, srv);
			break;
		case Shader::TessellationEvaluationShader:
			m_pDeviceContext->DSSetShaderResources(ID, 1, srv);
			break;
		case Shader::GeometryShader:
			m_pDeviceContext->GSSetShaderResources(ID, 1, srv);
			break;
		case Shader::PixelShader:
			m_pDeviceContext->PSSetShaderResources(ID, 1, srv);
			break;
		case Shader::ComputeShader:
			m_pDeviceContext->CSSetShaderResources(ID, 1, srv);
			break;
		}
	}
	else if (parameterType == ShaderParameter::Type::RWTexture)
	{
		switch (shaderType)
		{
		case Shader::ComputeShader:
			m_pDeviceContext->CSSetUnorderedAccessViews(ID, 1, uav, nullptr);
			break;
		}
	}*/
}