#pragma once

#include "Technique.h"


inline void Technique::Bind()
{
	m_Shader->bindShader();
}

inline void Technique::Unbind()
{
	m_Shader->unbindShader();
}


inline int32 Technique::getLocation(const char* name) const
{
    int32 location = m_Shader->getShaderConstLoc(name);
	if (location < 0)
	{
		Log::Error("Attrib [%s] not found in shader [%d]", name, m_Shader->m_ProgramGLObj);
	}
	return location;
}
//

inline void Technique::setTexture(const char* name, uint32 value) const
{
	m_Shader->setShaderSampler(getLocation(name), value);
}

inline void Technique::setTexture(int32 _loc, uint32 value) const
{
	m_Shader->setShaderSampler(_loc, value);
}

//

inline void Technique::setInt(const char* name, int32 value) const
{
	m_Shader->setShaderConst(getLocation(name), CONST_INT, &value);
}
inline void Technique::setInt(int32 _loc, int32 value) const
{
	m_Shader->setShaderConst(_loc, CONST_INT, &value);
}

inline void Technique::setFloat(const char* name, float value) const
{
	m_Shader->setShaderConst(getLocation(name), CONST_FLOAT, &value);
}
inline void Technique::setFloat(int32 _loc, float value) const
{
	m_Shader->setShaderConst(_loc, CONST_FLOAT, &value);
}

inline void Technique::setVec2(const char* name, cvec2 value) const
{
	m_Shader->setShaderConst(getLocation(name), CONST_FLOAT2, &value.x);
}
inline void Technique::setVec2(int32 _loc, cvec2 value) const
{
	m_Shader->setShaderConst(_loc, CONST_FLOAT2, &value.x);
}

inline void Technique::setVec3(const char* name, cvec3 value) const
{
	m_Shader->setShaderConst(getLocation(name), CONST_FLOAT3, &value.x);
}
inline void Technique::setVec3(int32 _loc, cvec3 value) const
{
	m_Shader->setShaderConst(_loc, CONST_FLOAT3, &value.x);
}

inline void Technique::setVec4(const char* name, cvec4 value) const
{
	m_Shader->setShaderConst(getLocation(name), CONST_FLOAT4, &value.x);
}
inline void Technique::setVec4(int32 _loc, cvec4 value) const
{
	m_Shader->setShaderConst(_loc, CONST_FLOAT4, &value.x);
}

//

inline void Technique::setMat4(const char* name, cmat4 mat) const
{
    setMat4(getLocation(name), mat);
}

inline void Technique::setMat4(int32 _loc, cmat4 mat) const
{
	m_Shader->setShaderConst(_loc, CONST_FLOAT44, &mat.x[0]);
}