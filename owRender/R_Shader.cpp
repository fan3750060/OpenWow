#include "stdafx.h"

// General
#include "R_Shader.h"

// Additional
#include "OpenGL.h"

R_Shader::R_Shader(RenderDevice* _RenderDevice, cstring _name) :
	m_Name(_name),
	m_ProgramGLObj(0),
	m_RenderDevice(_RenderDevice)
{}

R_Shader::~R_Shader()
{
	glDeleteProgram(m_ProgramGLObj);
}

void R_Shader::createShader(const char *vertexShaderSrc, const char *fragmentShaderSrc, const char *geometryShaderSrc, const char *tessControlShaderSrc, const char *tessEvaluationShaderSrc, const char *computeShaderSrc)
{
	// Compile and link shader
	m_ProgramGLObj = createShaderProgram(vertexShaderSrc, fragmentShaderSrc, geometryShaderSrc, tessControlShaderSrc, tessEvaluationShaderSrc, computeShaderSrc);
	if (m_ProgramGLObj == 0)
	{
		fail2(m_RenderDevice->getShaderLog().c_str());
	}

	if (!linkShaderProgram())
	{
		fail2(m_RenderDevice->getShaderLog().c_str());
	}

	int attribCount;
	glGetProgramiv(m_ProgramGLObj, GL_ACTIVE_ATTRIBUTES, &attribCount);

	for (uint32 i = 0; i < m_RenderDevice->_numVertexLayouts; ++i)
	{
		R_VertexLayout& vl = m_RenderDevice->m_VertexLayouts[i];
		bool allAttribsFound = true;

		for (uint32 j = 0; j < 16; ++j)
		{
			m_InputLayouts[i].attribIndices[j] = -1;
		}

		for (int j = 0; j < attribCount; ++j)
		{
			char name[32];
			uint32 size, type;
			glGetActiveAttrib(m_ProgramGLObj, j, 32, nullptr, (int *)&size, &type, name);

			bool attribFound = false;
			for (uint32 k = 0; k < vl.numAttribs; ++k)
			{
				if (vl.attribs[k].semanticName.compare(name) == 0)
				{
					m_InputLayouts[i].attribIndices[k] = glGetAttribLocation(m_ProgramGLObj, name);
					attribFound = true;
				}
			}

			if (!attribFound)
			{
				allAttribsFound = false;
				break;
			}
		}

		m_InputLayouts[i].valid = allAttribsFound;
	}
}

void R_Shader::bindShader()
{
	glUseProgram(m_ProgramGLObj);
}

void R_Shader::unbindShader()
{
	glUseProgram(0);
}

int R_Shader::getShaderConstLoc(const char *name)
{
	return glGetUniformLocation(m_ProgramGLObj, name);
}

int R_Shader::getShaderSamplerLoc(const char *name)
{
	return glGetUniformLocation(m_ProgramGLObj, name);
}

int R_Shader::getShaderBufferLoc(const char *name)
{
	if (GetSettingsGroup<CGroupRenderCaps>().computeShaders)
	{
		int idx = glGetProgramResourceIndex(m_ProgramGLObj, GL_SHADER_STORAGE_BLOCK, name);
		if (idx != -1)
		{
			const GLenum bufBindingPoint[1] = { GL_BUFFER_BINDING };
			glGetProgramResourceiv(m_ProgramGLObj, GL_SHADER_STORAGE_BLOCK, idx, 1, bufBindingPoint, 1, 0, &idx);
		}
		return idx;
	}
	else
	{
		Log::Error("Shader storage buffers are not supported on this device.");

		return -1;
	}
}

void R_Shader::setShaderConst(int loc, R_ShaderConstType type, const void* values, uint32 count)
{
	if (loc == -1)
	{
		return;
	}

	switch (type)
	{
	case CONST_INT:
		glUniform1iv(loc, count, (const int *)values);
		break;
	case CONST_FLOAT:
		glUniform1fv(loc, count, (const float *)values);
		break;
	case CONST_FLOAT2:
		glUniform2fv(loc, count, (const float *)values);
		break;
	case CONST_FLOAT3:
		glUniform3fv(loc, count, (const float *)values);
		break;
	case CONST_FLOAT4:
		glUniform4fv(loc, count, (const float *)values);
		break;
	case CONST_FLOAT44:
		glUniformMatrix4fv(loc, count, false, (const float *)values);
		break;
	case CONST_FLOAT33:
		glUniformMatrix3fv(loc, count, false, (const float *)values);
		break;
	}
}

void R_Shader::setShaderSampler(int loc, uint32 texUnit)
{
	if (loc == -1)
	{
		return;
	}

	glUniform1i(loc, (int)texUnit);
}

void R_Shader::runComputeShader(uint32 xDim, uint32 yDim, uint32 zDim)
{
	bindShader();

	m_RenderDevice->commitStates(nullptr, ~PM_GEOMETRY);
	{
		glDispatchCompute(xDim, yDim, zDim);
	}
}

// Helpers

uint32 R_Shader::createShaderProgram(const char *vertexShaderSrc, const char *fragmentShaderSrc, const char *geometryShaderSrc, const char *tessControlShaderSrc, const char *tessEvalShaderSrc, const char *computeShaderSrc)
{
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog = nullptr;
	int status;

	m_RenderDevice->m_ShaderLog = "";

	uint32 vs, fs, gs, tsC, tsE, cs;
	vs = fs = gs = tsC = tsE = cs = 0;

	// Vertex shader
	if (vertexShaderSrc)
	{
		vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertexShaderSrc, nullptr);
		glCompileShader(vs);
		glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			// Get info
			glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1)
			{
				infoLog = new char[infologLength];
				glGetShaderInfoLog(vs, infologLength, &charsWritten, infoLog);
				m_RenderDevice->m_ShaderLog = m_RenderDevice->m_ShaderLog + "[Vertex Shader]\n" + infoLog;
				SafeDeleteArray(infoLog);
			}

			glDeleteShader(vs);
			return 0;
		}
	}

	// Fragment shader
	if (fragmentShaderSrc)
	{
		fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragmentShaderSrc, nullptr);
		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1)
			{
				infoLog = new char[infologLength];
				glGetShaderInfoLog(fs, infologLength, &charsWritten, infoLog);
				m_RenderDevice->m_ShaderLog = m_RenderDevice->m_ShaderLog + "[Fragment Shader]\n" + infoLog;
				SafeDeleteArray(infoLog);
			}

			glDeleteShader(vs);
			glDeleteShader(fs);
			return 0;
		}
	}

	// Geometry shader
	if (geometryShaderSrc)
	{
		gs = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gs, 1, &geometryShaderSrc, nullptr);
		glCompileShader(gs);
		glGetShaderiv(gs, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			glGetShaderiv(gs, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1)
			{
				infoLog = new char[infologLength];
				glGetShaderInfoLog(gs, infologLength, &charsWritten, infoLog);
				m_RenderDevice->m_ShaderLog = m_RenderDevice->m_ShaderLog + "[Geometry Shader]\n" + infoLog;
				SafeDeleteArray(infoLog);
			}

			glDeleteShader(vs);
			glDeleteShader(fs);
			glDeleteShader(gs);
			return 0;
		}
	}

	// Tesselation control shader
	if (tessControlShaderSrc)
	{
		tsC = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tsC, 1, &tessControlShaderSrc, nullptr);
		glCompileShader(tsC);
		glGetShaderiv(tsC, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			glGetShaderiv(tsC, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1)
			{
				infoLog = new char[infologLength];
				glGetShaderInfoLog(tsC, infologLength, &charsWritten, infoLog);
				m_RenderDevice->m_ShaderLog = m_RenderDevice->m_ShaderLog + "[Tesselation Control Shader]\n" + infoLog;
				SafeDeleteArray(infoLog);
			}

			glDeleteShader(vs);
			glDeleteShader(fs);
			if (gs) glDeleteShader(gs);
			glDeleteShader(tsC);
			return 0;
		}
	}

	// Tesselation evaluation shader
	if (tessEvalShaderSrc)
	{
		tsE = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tsE, 1, &tessEvalShaderSrc, nullptr);
		glCompileShader(tsE);
		glGetShaderiv(tsE, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			glGetShaderiv(tsE, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1)
			{
				infoLog = new char[infologLength];
				glGetShaderInfoLog(tsE, infologLength, &charsWritten, infoLog);
				m_RenderDevice->m_ShaderLog = m_RenderDevice->m_ShaderLog + "[Tesselation Evaluation Shader]\n" + infoLog;
				SafeDeleteArray(infoLog);
			}

			glDeleteShader(vs);
			glDeleteShader(fs);
			if (gs) glDeleteShader(gs);
			glDeleteShader(tsC);
			glDeleteShader(tsE);
			return 0;
		}
	}

	// Comput evaluation shader
	if (computeShaderSrc)
	{
		cs = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(cs, 1, &computeShaderSrc, nullptr);
		glCompileShader(cs);
		glGetShaderiv(cs, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			glGetShaderiv(cs, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1)
			{
				infoLog = new char[infologLength];
				glGetShaderInfoLog(cs, infologLength, &charsWritten, infoLog);
				m_RenderDevice->m_ShaderLog = m_RenderDevice->m_ShaderLog + "[Compute Shader]\n" + infoLog;
				SafeDeleteArray(infoLog);
			}

			// other shader types should not be present in compute context, but better check
			if (vs) glDeleteShader(vs);
			if (fs) glDeleteShader(fs);
			if (gs) glDeleteShader(gs);
			if (tsC) glDeleteShader(tsC);
			if (tsE) glDeleteShader(tsE);
			glDeleteShader(cs);
			return 0;
		}
	}

	// Shader program
	uint32 program = glCreateProgram();
	if (vs && fs)
	{
		glAttachShader(program, vs);
		glAttachShader(program, fs);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	if (gs)
	{
		glAttachShader(program, gs);
		glDeleteShader(gs);
	}
	if (tsC)
	{
		glAttachShader(program, tsC);
		glDeleteShader(tsC);
	}
	if (tsE)
	{
		glAttachShader(program, tsE);
		glDeleteShader(tsE);
	}
	if (cs)
	{
		glAttachShader(program, cs);
		glDeleteShader(cs);
	}

	return program;
}

bool R_Shader::linkShaderProgram()
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog = nullptr;
	int status;

	m_RenderDevice->m_ShaderLog = "";

	glLinkProgram(m_ProgramGLObj);

	glGetProgramiv(m_ProgramGLObj, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(m_ProgramGLObj, infologLength, &charsWritten, infoLog);
		m_RenderDevice->m_ShaderLog = m_RenderDevice->m_ShaderLog + "[Linking]\n" + infoLog;
		SafeDeleteArray(infoLog);
	}

	glGetProgramiv(m_ProgramGLObj, GL_LINK_STATUS, &status);
	if (!status) return false;

	return true;
}