#pragma once

#include "Pipeline.h"
#include "Render.h"
#include "Technique.h"

inline void Technique::Bind()
{
	assert1(m_RenderDevice != nullptr);
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
		Log::Error("Attrib [%s] not found in shader [%d]", name, m_Shader->oglProgramObj);
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

//

inline void Technique::SetPVW()
{
	m_Shader->setShaderConst(gProjection, CONST_FLOAT44, &(_PipelineGlobal->GetProjection()).x[0]);
	m_Shader->setShaderConst(gView,       CONST_FLOAT44, &(_PipelineGlobal->GetView()).x[0]);
	m_Shader->setShaderConst(gWorld,      CONST_FLOAT44, &(_PipelineGlobal->GetWorld()).x[0]);
}

inline void Technique::SetPV()
{
	m_Shader->setShaderConst(gProjection, CONST_FLOAT44, &(_PipelineGlobal->GetProjection()).x[0]);
	m_Shader->setShaderConst(gView,       CONST_FLOAT44, &(_PipelineGlobal->GetView()).x[0]);
}

inline void Technique::SetW(cmat4 _mat)
{
	m_Shader->setShaderConst(gWorld, CONST_FLOAT44, &_mat.x[0]);
}

//--

inline void Technique::SetWorldMatrix(cmat4 WorldInverse)
{
	m_Shader->setShaderConst(gWorld, CONST_FLOAT44, &WorldInverse.x[0]);
}

inline void Technique::SetViewMatrix(cmat4 WorldInverse)
{
	m_Shader->setShaderConst(gView, CONST_FLOAT44, &WorldInverse.x[0]);
}

inline void Technique::SetProjectionMatrix(cmat4 WorldInverse)
{
	m_Shader->setShaderConst(gProjection, CONST_FLOAT44, &WorldInverse.x[0]);
}

//--

inline void Technique::BindToPostprocess()
{
	setTexture("gbuf0", 0);
	setTexture("gbuf1", 1);
	setTexture("gbuf2", 2);
	setTexture("gbuf3", 3);
}