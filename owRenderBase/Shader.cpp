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

bool Shader::HasSemantic(const BufferBinding& binding) const
{
	for (auto& it : m_InputSemantics)
	{
		if (it.first.Name == binding.Name && it.first.Index == binding.Index)
		{
			return true;
		}
	}

	return false;
}

const InputSemantic& Shader::GetSemantic(const BufferBinding& binding) const
{
	for (auto& it : m_InputSemantics)
	{
		if (it.first.Name == binding.Name && it.first.Index == binding.Index)
		{
			return it.first;
		}
	}

	_ASSERT(false);
	return gs_InvalidShaderSemantic;
}

UINT Shader::GetSemanticSlot(const BufferBinding& binding) const
{
	for (auto& it : m_InputSemantics)
	{
		if (it.first.Name == binding.Name && it.first.Index == binding.Index)
		{
			return it.second;
		}
	}

	_ASSERT(false);
	return UINT_MAX;
}