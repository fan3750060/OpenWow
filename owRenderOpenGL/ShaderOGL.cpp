#include <stdafx.h>

// Additional
#include "ShaderParameterOGL.h"

// General
#include "ShaderOGL.h"

// Additional
#include "OpenGL.h"

string RecursionInclude(std::shared_ptr<IFile> f)
{
	if (f == nullptr)
	{
		Log::Error("Error open shader [%s].", f->Path_Name().c_str());
		return "";
	}

	string data = "";

	while (!f->isEof())
	{
		string line;
		if (false == f->readLine(&line))
		{
			break;
		}

		// Skip empty lines
		if (line.length() == 0)
		{
			continue;
		}

		// Find directive
		if (line[0] == '#' && line[1] == 'i' && line[2] == 'n' && line[3] == 'c' && line[4] == 'l')
		{
			size_t firstBracketPosition = line.find('"');
			assert1(firstBracketPosition != string::npos);

			size_t lastBracketPosition = line.find_last_of('"');
			assert1(firstBracketPosition != lastBracketPosition);

			string inludeFileName = line.substr(firstBracketPosition + 1, lastBracketPosition - firstBracketPosition - 1);
			CFile::FixFilePath(inludeFileName);
			data += RecursionInclude(GetManager<IFilesManager>()->Open(f->Path() + inludeFileName)) + '\n';

			continue;
		}

		data += line + '\n';
	}

	return data;
}

// This parameter will be returned if an invalid shader parameter is requested.
static ShaderParameterOGL gs_InvalidShaderParameter;

GLenum TranslateShaderType(Shader::ShaderType _type)
{
	GLenum result = GL_VERTEX_SHADER;
	switch (_type)
	{
	case Shader::ShaderType::VertexShader:
		result = GL_VERTEX_SHADER;
		break;
	case Shader::ShaderType::PixelShader:
		result = GL_FRAGMENT_SHADER;
		break;
	case Shader::ShaderType::GeometryShader:
		result = GL_GEOMETRY_SHADER;
		break;
	case Shader::ShaderType::TessellationControlShader:
		result = GL_TESS_CONTROL_SHADER;
		break;
	case Shader::ShaderType::TessellationEvaluationShader:
		result = GL_TESS_EVALUATION_SHADER;
		break;
	case Shader::ShaderType::ComputeShader:
		result = GL_COMPUTE_SHADER;
		break;
	default:
		Log::Error("Unknown blend factor.");
	}

	return result;
}


ShaderOGL::ShaderOGL()
	: m_ShaderType(UnknownShaderType)
{
}

ShaderOGL::~ShaderOGL()
{
	Destroy();
}

bool ShaderOGL::GetShaderProgramLog(uint32 _obj, std::string * _errMsg)
{
	GLsizei infologLength = 0;
	glGetProgramiv(_obj, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 1)
	{
		GLsizei charsWritten = 0;
		_errMsg->resize(infologLength + 1);
		glGetProgramInfoLog(_obj, infologLength, &charsWritten, &(*_errMsg)[0]);
		return false;
	}

	GLint status;

	glGetProgramiv(_obj, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		return false;
	}

	glGetProgramiv(_obj, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
	{
		return false;
	}

	return true;
}

void ShaderOGL::Destroy()
{
	m_ShaderParameters.clear();
	m_InputSemantics.clear();
}

Shader::ShaderType ShaderOGL::GetType() const
{
	return m_ShaderType;
}

bool ShaderOGL::LoadShaderFromFile(ShaderType shaderType, cstring fileName, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile)
{
	std::shared_ptr<IFile> file = GetManager<IFilesManager>()->Open(fileName);

	string fileSource = RecursionInclude(file);

	char* sourceCST = new char[fileSource.size() + 1];
	strcpy_s(sourceCST, fileSource.size() + 1, fileSource.c_str());

	m_GLShaderProgramm = glCreateShaderProgramv(TranslateShaderType(shaderType), 1, &sourceCST);
	_ASSERT(m_GLShaderProgramm != 0);
	OGLCheckError();
	std::string errMsg;
	if (false == GetShaderProgramLog(m_GLShaderProgramm, &errMsg))
	{
		fail2(errMsg.c_str());
		SafeDeleteArray(sourceCST);
		return false;
	}

	GLint attribCount;
	glGetProgramiv(m_GLShaderProgramm, GL_ACTIVE_ATTRIBUTES, &attribCount);
	for (GLuint j = 0; j < attribCount; ++j)
	{
		char name[32];
		GLsizei length, size;
		GLenum type;
		glGetActiveAttrib(m_GLShaderProgramm, j, 32, &length, &size, &type, name);
		/* Type
		GL_FLOAT, GL_FLOAT_VEC2, GL_FLOAT_VEC3, GL_FLOAT_VEC4, 
		GL_FLOAT_MAT2, GL_FLOAT_MAT3, GL_FLOAT_MAT4, 
		GL_FLOAT_MAT2x3, GL_FLOAT_MAT2x4, 
		GL_FLOAT_MAT3x2, GL_FLOAT_MAT3x4, 
		GL_FLOAT_MAT4x2, GL_FLOAT_MAT4x3,
		GL_INT, GL_INT_VEC2, GL_INT_VEC3, GL_INT_VEC4, 
		GL_UNSIGNED_INT, GL_UNSIGNED_INT_VEC2, GL_UNSIGNED_INT_VEC3, GL_UNSIGNED_INT_VEC4, 
		GL_DOUBLE, GL_DOUBLE_VEC2, GL_DOUBLE_VEC3, GL_DOUBLE_VEC4, 
		GL_DOUBLE_MAT2, GL_DOUBLE_MAT3, GL_DOUBLE_MAT4, 
		GL_DOUBLE_MAT2x3, GL_DOUBLE_MAT2x4, 
		GL_DOUBLE_MAT3x2, GL_DOUBLE_MAT3x4, 
		GL_DOUBLE_MAT4x2, or GL_DOUBLE_MAT4x3*/
		OGLCheckError();
		m_InputSemantics.insert(SemanticMap::value_type(BufferBinding(name, j), j));
	}

	GLint uniformsCount;
	glGetProgramiv(m_GLShaderProgramm, GL_ACTIVE_UNIFORMS, &uniformsCount);
	for (GLuint j = 0; j < uniformsCount; ++j)
	{
		char name[32];
		GLsizei length, size;
		GLenum type;
		glGetActiveUniform(m_GLShaderProgramm, j, 32, &length, &size, &type, name);
		// Types https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
		OGLCheckError();

		ShaderParameter::Type parameterType = ShaderParameter::Type::Invalid;
		switch (type)
		{
		case GL_IMAGE_2D:
			parameterType = ShaderParameter::Type::Texture;
			break;
		case GL_SAMPLER_2D:
			parameterType = ShaderParameter::Type::Sampler;
			break;
		case GL_SAMPLER_BUFFER:
		case GL_IMAGE_BUFFER:
			parameterType = ShaderParameter::Type::Buffer;
			break;
		default:
			Log::Warn("Unknown shader parameter [%s] [%s]", name, fileName.c_str());
			break;
		}

		// Create an empty shader parameter that should be filled-in by the application.
		std::shared_ptr<ShaderParameterOGL> shaderParameter = std::make_shared<ShaderParameterOGL>(name, j, shaderType, parameterType);
		m_ShaderParameters.insert(ParameterMap::value_type(name, shaderParameter));
	}


	SafeDeleteArray(sourceCST);
	return true;
}

ShaderParameter& ShaderOGL::GetShaderParameterByName(cstring name) const
{
	ParameterMap::const_iterator iter = m_ShaderParameters.find(name);
	if (iter != m_ShaderParameters.end())
	{
		return *(iter->second);
	}

	return gs_InvalidShaderParameter;
}

bool ShaderOGL::HasSemantic(const BufferBinding& binding) const
{
	SemanticMap::const_iterator iter = m_InputSemantics.find(binding);
	return iter != m_InputSemantics.end();
}

UINT ShaderOGL::GetSlotIDBySemantic(const BufferBinding& binding) const
{
	SemanticMap::const_iterator iter = m_InputSemantics.find(binding);
	if (iter != m_InputSemantics.end())
	{
		return iter->second;
	}

	// Some kind of error code or exception...
	return (UINT)-1;
}

void ShaderOGL::Bind()
{
	for (ParameterMap::value_type value : m_ShaderParameters)
	{
		value.second->Bind();
	}

	glUseProgram(m_GLShaderProgramm);
}

void ShaderOGL::UnBind()
{
	for (ParameterMap::value_type value : m_ShaderParameters)
	{
		value.second->UnBind();
	}

	glUseProgram(0);
}

void ShaderOGL::Dispatch(const glm::uvec3& numGroups)
{
	glDispatchCompute(numGroups.x, numGroups.y, numGroups.z);
}

uint32 ShaderOGL::GetGLObject()
{
	return m_GLShaderProgramm;
}
