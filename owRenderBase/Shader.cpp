#include "stdafx.h"

// Include
#include "ShaderParameter.h"
#include "BufferBinding.h"

// General
#include "Shader.h"

static ShaderParameter gs_InvalidShaderParameter;
static InputSemantic gs_InvalidShaderSemantic;

//--------------------------------------------------------------------------------------------

Shader::Shader() :
	m_ShaderType(ShaderType::UnknownShaderType)
{
}

Shader::ShaderType Shader::GetType() const
{
	return m_ShaderType;
}

ShaderParameter& Shader::GetShaderParameterByName(cstring name) const
{
	ParameterMap::const_iterator iter = m_ShaderParameters.find(name);
	if (iter != m_ShaderParameters.end())
	{
		return *(iter->second);
	}

	//_ASSERT(false);
	return gs_InvalidShaderParameter;
}

bool Shader::HasSemantic(cstring binding) const
{
	for (auto& it : m_InputSemantics)
	{
		if (it.first.Name == binding)
		{
			return true;
		}
	}

	return false;
}

const InputSemantic& Shader::GetSemantic(cstring binding) const
{
	for (auto& it : m_InputSemantics)
	{
		if (it.first.Name == binding)
		{
			return it.first;
		}
	}

	_ASSERT(false);
	return gs_InvalidShaderSemantic;
}

UINT Shader::GetSemanticSlot(cstring binding) const
{
	for (auto& it : m_InputSemantics)
	{
		if (it.first.Name == binding)
		{
			return it.second;
		}
	}

	_ASSERT(false);
	return UINT_MAX;
}