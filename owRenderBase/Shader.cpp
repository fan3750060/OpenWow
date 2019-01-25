#include "stdafx.h"

// Include
#include "ShaderParameter.h"
#include "BufferBinding.h"

// General
#include "Shader.h"

Shader::Shader() :
	m_ShaderType(ShaderType::UnknownShaderType)
{
}

Shader::ShaderType Shader::GetType() const
{
	return m_ShaderType;
}
