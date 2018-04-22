#pragma once

#include "Pipeline.h"
#include "Render.h"
#include "Technique.h"

inline void Technique::InitBaseUniforms()
{
    gProjection = getLocation("gProjection");
    gView = getLocation("gView");
    gWorld = getLocation("gWorld");
}

inline void Technique::BindS()
{
	_Render->r->bindShader(shader);
}

inline void Technique::Unbind()
{
	_Render->r->bindShader(0);
}


inline int32 Technique::getLocation(const char* name) const
{
    int32 location = _Render->r->getShaderConstLoc(shader, name);
    Log::Error("Attrib [%s] not found in shader [%d]", name, shader->oglProgramObj);
	return location;
}
//

inline void Technique::setTexture(const char* name, uint32 value) const
{
	_Render->r->setShaderSampler(getLocation(name), value);
}

inline void Technique::setTexture(int32 _loc, uint32 value) const
{
    _Render->r->setShaderSampler(_loc, value);
}

//

inline void Technique::setInt(const char* name, int32 value) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_INT, &value);
}
inline void Technique::setInt(int32 _loc, int32 value) const
{
    _Render->r->setShaderConst(_loc, CONST_INT, &value);
}

inline void Technique::setFloat(const char* name, float value) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT, &value);
}
inline void Technique::setFloat(int32 _loc, float value) const
{
    _Render->r->setShaderConst(_loc, CONST_FLOAT, &value);
}

inline void Technique::setVec2(const char* name, cvec2 value) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT2, &value.x);
}
inline void Technique::setVec2(int32 _loc, cvec2 value) const
{
    _Render->r->setShaderConst(_loc, CONST_FLOAT2, &value.x);
}

inline void Technique::setVec3(const char* name, cvec3 value) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT3, &value.x);
}
inline void Technique::setVec3(int32 _loc, cvec3 value) const
{
    _Render->r->setShaderConst(_loc, CONST_FLOAT3, &value.x);
}

inline void Technique::setVec4(const char* name, cvec4 value) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT4, &value.x);
}
inline void Technique::setVec4(int32 _loc, cvec4 value) const
{
    _Render->r->setShaderConst(_loc, CONST_FLOAT4, &value.x);
}

//

inline void Technique::setMat4(const char* name, cmat4 mat) const
{
    setMat4(getLocation(name), mat);
}

inline void Technique::setMat4(int32 _loc, cmat4 mat) const
{
    _Render->r->setShaderConst(_loc, CONST_FLOAT44, &mat.x[0]);
}

//

inline void Technique::SetPVW()
{
    _Render->r->setShaderConst(gProjection, CONST_FLOAT44, &_PipelineGlobal->GetProjection().x[0]);
    _Render->r->setShaderConst(gView,       CONST_FLOAT44, &_PipelineGlobal->GetView().x[0]);
    _Render->r->setShaderConst(gWorld,      CONST_FLOAT44, &_PipelineGlobal->GetWorld().x[0]);
}

inline void Technique::SetPV()
{
    _Render->r->setShaderConst(gProjection, CONST_FLOAT44, &_PipelineGlobal->GetProjection().x[0]);
    _Render->r->setShaderConst(gView,       CONST_FLOAT44, &_PipelineGlobal->GetView().x[0]);
}

inline void Technique::SetW(cmat4 _mat)
{
    _Render->r->setShaderConst(gWorld, CONST_FLOAT44, &_mat.x[0]);
}

//--

inline void Technique::SetWorldMatrix(cmat4 WorldInverse)
{
    _Render->r->setShaderConst(gWorld, CONST_FLOAT44, &WorldInverse.x[0]);
}

inline void Technique::SetViewMatrix(cmat4 WorldInverse)
{
    _Render->r->setShaderConst(gView, CONST_FLOAT44, &WorldInverse.x[0]);
}

inline void Technique::SetProjectionMatrix(cmat4 WorldInverse)
{
    _Render->r->setShaderConst(gProjection, CONST_FLOAT44, &WorldInverse.x[0]);
}

//--

inline void Technique::BindToPostprocess()
{
	setTexture("gbuf0", 0);
	setTexture("gbuf1", 1);
	setTexture("gbuf2", 2);
	setTexture("gbuf3", 3);
}