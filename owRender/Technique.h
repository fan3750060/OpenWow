#pragma once

#include "RenderDevice.h"

// FORWARD BEGIN
class TechniquesManager;
// FORWARD END

class Technique : public CRefItem
{
	friend TechniquesManager;
protected:
	Technique(RenderDevice* _RenderDevice, cstring _fileName);
	Technique(RenderDevice* _RenderDevice, cstring _fileNameVS, cstring _fileNameFS);
    Technique(RenderDevice* _RenderDevice, cstring _fileNameVS, cstring _fileNameFS, cstring _filenameGS);

    void Process(cstring _name, const char *vertexShaderSrc, const char *fragmentShaderSrc, const char *geometryShaderSrc);

public:
	inline void Bind();
	inline void Unbind();

protected:
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

protected:
	SharedPtr<R_Shader>  m_Shader;
	RenderDevice*		m_RenderDevice;
};

typedef SharedPtr<Technique> SharedTechPtr;

#include "Technique.inl"