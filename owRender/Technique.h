#pragma once

#include "RenderDevice.h"

class TechniquesManager;

class Technique
{
	friend TechniquesManager;
protected:
	Technique(RenderDevice* _RenderDevice, cstring _fileName);
	Technique(RenderDevice* _RenderDevice, cstring _fileNameVS, cstring _fileNameFS);
    Technique(RenderDevice* _RenderDevice, cstring _fileNameVS, cstring _fileNameFS, cstring _filenameGS);
	~Technique();

    void Process(cstring fileName, const char *vertexShaderSrc, const char *fragmentShaderSrc, const char *geometryShaderSrc);

public:
	inline void Bind();
	inline void Unbind();

protected:
	void InitBaseUniforms();
	inline int32 getLocation(const char* name) const;

	inline void setTexture(const char* name, uint32 value) const;
    inline void setTexture(int32 _loc, uint32 value) const;

	inline void setInt(const char* name, int32 value) const;
    inline void setInt(int32 _loc, int32 value) const;
	inline void setFloat(const char* name, float value) const;
    inline void setFloat(int32 _loc, float value) const;
	inline void setVec2(const char* name, cvec2 value) const;
    inline void setVec2(int32 _loc, cvec2 value) const;
	inline void setVec3(const char* name, cvec3 value) const;
    inline void setVec3(int32 _loc, cvec3 value) const;
	inline void setVec4(const char* name, cvec4 value) const;
    inline void setVec4(int32 _loc, cvec4 value) const;

	inline void setMat4(const char* name, cmat4 mat) const;
    inline void setMat4(int32 _loc, cmat4 mat) const;

public:
	inline void SetPVW();
    inline void SetPV();
    inline void SetW(cmat4 _mat);

	inline void SetWorldMatrix(cmat4 WorldInverse);
	inline void SetViewMatrix(cmat4 WorldInverse);
	inline void SetProjectionMatrix(cmat4 WorldInverse);

	inline void BindToPostprocess();

protected: // Base uniforms
    int32 gProjection;
    int32 gView;
    int32 gWorld;

protected:
	R_Shader*      m_Shader;
	RenderDevice*  m_RenderDevice;
};

#include "Technique.inl"