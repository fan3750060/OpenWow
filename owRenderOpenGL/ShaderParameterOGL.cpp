#include <stdafx.h>

#include "ShaderParameterOGL.h"

// Additional
#include "ConstantBufferOGL.h"
#include "TextureOGL.h"
#include "SamplerStateOGL.h"
#include "StructuredBufferOGL.h"
#include "OpenGL.h"

ShaderParameterOGL::ShaderParameterOGL()
	: m_uiSlotID(UINT_MAX)
	, m_ParameterType(Type::Invalid)
{}

ShaderParameterOGL::ShaderParameterOGL(cstring name, UINT slotID, Shader::ShaderType shaderType, Type parameterType)
	: m_Name(name)
	, m_uiSlotID(slotID)
	, m_ShaderType(shaderType)
	, m_ParameterType(parameterType)
{}

void ShaderParameterOGL::SetConstantBuffer(std::shared_ptr<ConstantBuffer> buffer)
{
	m_pConstantBuffer = buffer;
}

void ShaderParameterOGL::SetTexture(std::shared_ptr<Texture> texture)
{
	m_pTexture = texture;
}

void ShaderParameterOGL::SetSampler(std::shared_ptr<SamplerState> sampler)
{
	m_pSamplerState = sampler;
}

void ShaderParameterOGL::SetStructuredBuffer(std::shared_ptr<StructuredBuffer> rwBuffer)
{
	m_pStructuredBuffer = rwBuffer;
}

bool ShaderParameterOGL::IsValid() const
{
	return m_ParameterType != ShaderParameter::Type::Invalid;
}

ShaderParameter::Type ShaderParameterOGL::GetType() const
{
	return m_ParameterType;
}

void ShaderParameterOGL::Bind()
{
	if (std::shared_ptr<ConstantBuffer> constantBuffer = m_pConstantBuffer.lock())
	{
		constantBuffer->Bind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
	if (std::shared_ptr<Texture> texture = m_pTexture.lock())
	{
		texture->Bind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
	if (std::shared_ptr<SamplerState> samplerState = m_pSamplerState.lock())
	{
		samplerState->Bind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
	if (std::shared_ptr<StructuredBuffer> buffer = m_pStructuredBuffer.lock())
	{
		buffer->Bind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
}

void ShaderParameterOGL::UnBind()
{
	if (std::shared_ptr<ConstantBuffer> constantBuffer = m_pConstantBuffer.lock())
	{
		constantBuffer->UnBind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
	if (std::shared_ptr<Texture> texture = m_pTexture.lock())
	{
		texture->UnBind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
	if (std::shared_ptr<SamplerState> samplerState = m_pSamplerState.lock())
	{
		samplerState->UnBind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
	if (std::shared_ptr<StructuredBuffer> buffer = m_pStructuredBuffer.lock())
	{
		buffer->UnBind(m_uiSlotID, m_ShaderType, m_ParameterType);
	}
}
