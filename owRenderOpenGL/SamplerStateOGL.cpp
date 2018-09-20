#include <stdafx.h>

// General
#include "SamplerStateOGL.h"

// Additional
#include "OpenGL.h"

GLenum TranslateMinMapFilter(SamplerStateOGL::MinFilter _minFilter, SamplerStateOGL::MipFilter _mipFilter)
{
	GLenum filter = GL_LINEAR_MIPMAP_LINEAR;

	if (_minFilter == SamplerStateOGL::MinFilter::MinLinear && _mipFilter == SamplerStateOGL::MipFilter::MipLinear)
	{
		filter = GL_LINEAR_MIPMAP_LINEAR;
	}
	else if (_minFilter == SamplerStateOGL::MinFilter::MinLinear && _mipFilter == SamplerStateOGL::MipFilter::MipNearest)
	{
		filter = GL_LINEAR_MIPMAP_NEAREST;
	}
	else if (_minFilter == SamplerStateOGL::MinFilter::MinNearest && _mipFilter == SamplerStateOGL::MipFilter::MipLinear)
	{
		filter = GL_NEAREST_MIPMAP_LINEAR;
	}
	else if (_minFilter == SamplerStateOGL::MinFilter::MinNearest && _mipFilter == SamplerStateOGL::MipFilter::MipNearest)
	{
		filter = GL_NEAREST_MIPMAP_NEAREST;
	}
	else
	{
		Log::Error("Invalid texture filter modes.");
	}

	return filter;
}

GLenum TranslateMagFilter(SamplerStateOGL::MagFilter _magFilter)
{
	GLenum filter = GL_LINEAR;
	if (_magFilter == SamplerStateOGL::MagFilter::MagLinear)
	{
		filter = GL_LINEAR;
	}
	else if (_magFilter == SamplerStateOGL::MagFilter::MagLinear)
	{
		filter = GL_NEAREST;
	}
	else
	{
		Log::Error("Invalid texture filter modes.");
	}

	return filter;
}

GLenum TranslateWrapMode(SamplerStateOGL::WrapMode wrapMode)
{
	GLenum addressMode = GL_REPEAT;

	switch (wrapMode)
	{
	case SamplerStateOGL::WrapMode::Repeat:
		addressMode = GL_REPEAT;
		break;
	case SamplerStateOGL::WrapMode::Clamp:
		addressMode = GL_CLAMP_TO_EDGE;
		break;
	case SamplerStateOGL::WrapMode::Mirror:
		addressMode = GL_MIRRORED_REPEAT;
		break;
	case SamplerStateOGL::WrapMode::Border:
		addressMode = GL_CLAMP_TO_BORDER;
		break;
	}

	return addressMode;
}

GLenum TranslateCompareMode(SamplerStateOGL::CompareMode mode)
{
	GLenum result = SamplerStateOGL::CompareMode::None;

	switch (result)
	{
	case SamplerStateOGL::CompareMode::None:
		result = GL_NONE;
		break;
	case SamplerStateOGL::CompareMode::CompareRefToTexture:
		result = GL_COMPARE_R_TO_TEXTURE;
		break;
	default:
		Log::Error("Unknown compare mode.");
		break;
	}

	return result;
}

GLenum TranslateCompareFunction(SamplerStateOGL::CompareFunc compareFunc)
{
	GLenum compareFuncD3D11 = GL_ALWAYS;
	switch (compareFunc)
	{
	case SamplerStateOGL::CompareFunc::Never:
		compareFuncD3D11 = GL_NEVER;
		break;
	case SamplerStateOGL::CompareFunc::Less:
		compareFuncD3D11 = GL_LESS;
		break;
	case SamplerStateOGL::CompareFunc::Equal:
		compareFuncD3D11 = GL_EQUAL;
		break;
	case SamplerStateOGL::CompareFunc::LessEqual:
		compareFuncD3D11 = GL_LEQUAL;
		break;
	case SamplerStateOGL::CompareFunc::Greater:
		compareFuncD3D11 = GL_GREATER;
		break;
	case SamplerStateOGL::CompareFunc::NotEqual:
		compareFuncD3D11 = GL_NOTEQUAL;
		break;
	case SamplerStateOGL::CompareFunc::GreaterEqual:
		compareFuncD3D11 = GL_GEQUAL;
		break;
	case SamplerStateOGL::CompareFunc::Always:
		compareFuncD3D11 = GL_ALWAYS;
		break;
	}

	return compareFuncD3D11;
}

SamplerStateOGL::SamplerStateOGL()
	: m_MinFilter(MinFilter::MinNearest)
	, m_MagFilter(SamplerStateOGL::MagFilter::MagNearest)
	, m_MipFilter(SamplerStateOGL::MipFilter::MipNearest)
	, m_WrapModeU(WrapMode::Repeat)
	, m_WrapModeV(WrapMode::Repeat)
	, m_WrapModeW(WrapMode::Repeat)
	, m_CompareMode(CompareMode::None)
	, m_CompareFunc(CompareFunc::Always)
	, m_fLODBias(0.0f)
	, m_fMinLOD(0.0f)
	, m_fMaxLOD(Math::MaxFloat)
	, m_bIsAnisotropicFilteringEnabled(false)
	, m_AnisotropicFiltering(1)
	, m_bIsDirty(true)
{
}

SamplerStateOGL::~SamplerStateOGL()
{}

void SamplerStateOGL::SetFilter(MinFilter minFilter, MagFilter magFilter, MipFilter mipFilter)
{
	m_MinFilter = minFilter;
	m_MagFilter = magFilter;
	m_MipFilter = mipFilter;
	m_bIsDirty = true;
}

void SamplerStateOGL::GetFilter(MinFilter& minFilter, MagFilter& magFilter, MipFilter& mipFilter) const
{
	minFilter = m_MinFilter;
	magFilter = m_MagFilter;
	mipFilter = m_MipFilter;
}

void SamplerStateOGL::SetWrapMode(WrapMode u, WrapMode v, WrapMode w)
{
	m_WrapModeU = u;
	m_WrapModeV = v;
	m_WrapModeW = w;
	m_bIsDirty = true;
}

void SamplerStateOGL::GetWrapMode(WrapMode& u, WrapMode& v, WrapMode& w) const
{
	u = m_WrapModeU;
	v = m_WrapModeV;
	w = m_WrapModeW;
}

void SamplerStateOGL::SetCompareFunction(CompareFunc compareFunc)
{
	m_CompareFunc = compareFunc;
	m_bIsDirty = true;
}

SamplerState::CompareFunc SamplerStateOGL::GetCompareFunc() const
{
	return m_CompareFunc;
}

void SamplerStateOGL::SetLODBias(float lodBias)
{
	m_fLODBias = lodBias;
	m_bIsDirty = true;
}

float SamplerStateOGL::GetLODBias() const
{
	return m_fLODBias;
}

void SamplerStateOGL::SetMinLOD(float minLOD)
{
	m_fMinLOD = minLOD;
	m_bIsDirty = true;
}

float SamplerStateOGL::GetMinLOD() const
{
	return m_fMinLOD;
}

void SamplerStateOGL::SetMaxLOD(float maxLOD)
{
	m_fMaxLOD = maxLOD;
	m_bIsDirty = true;
}

float SamplerStateOGL::GetMaxLOD() const
{
	return m_fMaxLOD;
}

void SamplerStateOGL::SetBorderColor(cvec4 borderColor)
{
	m_BorderColor = borderColor;
	m_bIsDirty = true;
}

vec4 SamplerStateOGL::GetBorderColor() const
{
	return m_BorderColor;
}

void SamplerStateOGL::EnableAnisotropicFiltering(bool enabled)
{
	m_bIsAnisotropicFilteringEnabled = enabled;
	m_bIsDirty = true;
}

bool SamplerStateOGL::IsAnisotropicFilteringEnabled() const
{
	return m_bIsAnisotropicFilteringEnabled;
}


void SamplerStateOGL::SetMaxAnisotropy(uint8_t maxAnisotropy)
{
	m_AnisotropicFiltering = glm::clamp<uint8_t>(maxAnisotropy, 1, 16);
	m_bIsDirty = true;
}

uint8_t SamplerStateOGL::GetMaxAnisotropy() const
{
	return m_AnisotropicFiltering;
}

void SamplerStateOGL::Bind(uint32_t ID, Shader::ShaderType shaderType, ShaderParameter::Type parameterType)
{
	//if (m_bIsDirty)
	{
		glTexParameteri(ID, GL_TEXTURE_MIN_FILTER, TranslateMinMapFilter(m_MinFilter, m_MipFilter));
		glTexParameteri(ID, GL_TEXTURE_MAG_FILTER, TranslateMagFilter(m_MagFilter));

		glTexParameteri(ID, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_AnisotropicFiltering);

		// Lod
		glTexParameterf(ID, GL_TEXTURE_MIN_LOD, m_fMinLOD);
		glTexParameterf(ID, GL_TEXTURE_MAX_LOD, m_fMaxLOD);

		// Wrap
		glTexParameteri(ID, GL_TEXTURE_WRAP_S, TranslateWrapMode(m_WrapModeU));
		glTexParameteri(ID, GL_TEXTURE_WRAP_T, TranslateWrapMode(m_WrapModeV));
		glTexParameteri(ID, GL_TEXTURE_WRAP_R, TranslateWrapMode(m_WrapModeW));

		if (m_CompareMode != CompareMode::None)
		{
			glTexParameteri(ID, GL_TEXTURE_COMPARE_MODE, TranslateCompareMode(m_CompareMode));
			glTexParameteri(ID, GL_TEXTURE_COMPARE_FUNC, TranslateCompareFunction(m_CompareFunc));
		}
		else
		{
			glTexParameteri(ID, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		}

		m_bIsDirty = false;
	}

	/*ID3D11SamplerState* pSamplers[] = { m_pSamplerState };

	switch (shaderType)
	{
	case Shader::VertexShader:
		m_pDeviceContext->VSSetSamplers(ID, 1, pSamplers);
		break;
	case Shader::TessellationControlShader:
		m_pDeviceContext->HSSetSamplers(ID, 1, pSamplers);
		break;
	case Shader::TessellationEvaluationShader:
		m_pDeviceContext->DSSetSamplers(ID, 1, pSamplers);
		break;
	case Shader::GeometryShader:
		m_pDeviceContext->GSSetSamplers(ID, 1, pSamplers);
		break;
	case Shader::PixelShader:
		m_pDeviceContext->PSSetSamplers(ID, 1, pSamplers);
		break;
	case Shader::ComputeShader:
		m_pDeviceContext->CSSetSamplers(ID, 1, pSamplers);
		break;
	}*/
}

void SamplerStateOGL::UnBind(uint32_t ID, Shader::ShaderType shaderType, ShaderParameter::Type parameterType)
{
	/*ID3D11SamplerState* pSamplers[] = { nullptr };

	switch (shaderType)
	{
	case Shader::VertexShader:
		m_pDeviceContext->VSSetSamplers(ID, 1, pSamplers);
		break;
	case Shader::TessellationControlShader:
		m_pDeviceContext->HSSetSamplers(ID, 1, pSamplers);
		break;
	case Shader::TessellationEvaluationShader:
		m_pDeviceContext->DSSetSamplers(ID, 1, pSamplers);
		break;
	case Shader::GeometryShader:
		m_pDeviceContext->GSSetSamplers(ID, 1, pSamplers);
		break;
	case Shader::PixelShader:
		m_pDeviceContext->PSSetSamplers(ID, 1, pSamplers);
		break;
	case Shader::ComputeShader:
		m_pDeviceContext->CSSetSamplers(ID, 1, pSamplers);
		break;
	}*/
}

