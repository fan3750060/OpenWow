#pragma once

// FORWARD BEGIN
class RenderDevice;
// FORWARD END

class R_Shader : public CRefItem
{
public:
	R_Shader(RenderDevice* _RenderDevice, cstring _name);
	~R_Shader();
	
	void createShader(const char *vertexShaderSrc, const char *fragmentShaderSrc, const char *geometryShaderSrc, const char *tessControlShaderSrc, const char *tessEvaluationShaderSrc, const char *computeShaderSrc);

	void bindShader();
	void unbindShader();
	int getShaderConstLoc(const char* name);
	int getShaderSamplerLoc(const char* name);
	int getShaderBufferLoc(const char* name);
	void setShaderConst(int loc, R_ShaderConstType type, const void *values, uint32 count = 1);
	void setShaderSampler(int loc, uint32 texUnit);
	void runComputeShader(uint32 xDim, uint32 yDim, uint32 zDim);

private:
	uint32 createShaderProgram(const char *vertexShaderSrc, const char *fragmentShaderSrc, const char *geometryShaderSrc, const char *tessControlShaderSrc, const char *tessEvalShaderSrc, const char *computeShaderSrc);
	bool linkShaderProgram();

public:
	string				m_Name;
	uint32				m_ProgramGLObj;
	R_InputLayout		m_InputLayouts[MaxNumVertexLayouts];

private:
	RenderDevice*		m_RenderDevice;
};