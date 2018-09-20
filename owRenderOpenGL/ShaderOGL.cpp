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
		f->readLine(&line);

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

void ShaderOGL::Destroy()
{
	m_ShaderParameters.clear();
	m_InputSemantics.clear();
}

Shader::ShaderType ShaderOGL::GetType() const
{
	return m_ShaderType;
}

bool ShaderOGL::LoadShaderFromString(ShaderType shaderType, cstring source, cstring sourceFileName, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile)
{
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog = nullptr;
	int status;

	char* sourceCST = new char[2048];
	strcpy_s(sourceCST, 2048, source.c_str());

	m_GLShaderProgramm = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &sourceCST);
	glGetShaderiv(m_GLShaderProgramm, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		// Get info
		glGetShaderiv(m_GLShaderProgramm, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 1)
		{
			infoLog = new char[infologLength];
			glGetShaderInfoLog(m_GLShaderProgramm, infologLength, &charsWritten, infoLog);

			SafeDeleteArray(infoLog);
		}

		glDeleteProgram(m_GLShaderProgramm);
		return false;
	}





	HRESULT hr;
	{
		/*ATL::CComPtr<ID3DBlob> pShaderBlob;
		ATL::CComPtr<ID3DBlob> pErrorBlob;

		std::vector<D3D_SHADER_MACRO> macros;
		for (auto macro : shaderMacros)
		{
			// The macro definitions passed to this function only store temporary string objects.
			// I need to copy the temporary strings into the D3D macro type 
			// in order for it to persist outside of this for loop.
			std::string name = macro.first;
			std::string definition = macro.second;

			char* c_name = new char[name.size() + 1];
			char* c_definition = new char[definition.size() + 1];

			strncpy_s(c_name, name.size() + 1, name.c_str(), name.size());
			strncpy_s(c_definition, definition.size() + 1, definition.c_str(), definition.size());

			macros.push_back({ c_name, c_definition });
		}
		macros.push_back({ 0, 0 });


		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif*/

		// fs::path filePath( sourceFileName );
		 //std::string sourceFilePath = filePath.string();

		//hr = D3DCompile((LPCVOID)source.c_str(), source.size(), sourceFileName.c_str(), macros.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), _profile.c_str(), flags, 0, &pShaderBlob, &pErrorBlob);

		// We're done compiling.. Delete the macro definitions.
		/*for (D3D_SHADER_MACRO macro : macros)
		{
			delete[] macro.Name;
			delete[] macro.Definition;
		}
		macros.clear();*/

		/*if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				OutputDebugString(static_cast<char*>(pErrorBlob->GetBufferPointer()));
				Log::Error(static_cast<char*>(pErrorBlob->GetBufferPointer()));
			}
			return false;
		}*/

		//m_pShaderBlob = pShaderBlob;
	}

	// After the shader recompiles, try to restore the shader parameters.
	ParameterMap shaderParameters = m_ShaderParameters;

	// Destroy the last shader as we are now loading a new one.
	Destroy();

	m_ShaderType = shaderType;

	/*switch (m_ShaderType)
	{
	case VertexShader:
		hr = m_pDevice->CreateVertexShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pVertexShader);
		break;
	case TessellationControlShader:
		hr = m_pDevice->CreateHullShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pHullShader);
		break;
	case TessellationEvaluationShader:
		hr = m_pDevice->CreateDomainShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pDomainShader);
		break;
	case GeometryShader:
		hr = m_pDevice->CreateGeometryShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pGeometryShader);
		break;
	case PixelShader:
		hr = m_pDevice->CreatePixelShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pPixelShader);
		break;
	case ComputeShader:
		hr = m_pDevice->CreateComputeShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pComputeShader);
		break;
	default:
		Log::Error("Invalid shader type.");
		break;
	}*/

	/*if (FAILED(hr))
	{
		Log::Error("Failed to create shader.");
		return false;
	}*/

	// Reflect the parameters from the shader.
	// Inspired by: http://members.gamedev.net/JasonZ/Heiroglyph/D3D11ShaderReflection.pdf
	/*ATL::CComPtr<ID3D11ShaderReflection> pReflector;
	hr = D3DReflect(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);

	if (FAILED(hr))
	{
		Log::Error("Failed to reflect shader.");
		return false;
	}*/

	// Query input parameters and build the input layout
	/*D3D11_SHADER_DESC shaderDescription;
	hr = pReflector->GetDesc(&shaderDescription);

	if (FAILED(hr))
	{
		Log::Error("Failed to get shader description from shader reflector.");
		return false;
	}

	m_InputSemantics.clear();

	UINT numInputParameters = shaderDescription.InputParameters;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
	for (UINT i = 0; i < numInputParameters; ++i)
	{
		D3D11_INPUT_ELEMENT_DESC inputElement;
		D3D11_SIGNATURE_PARAMETER_DESC parameterSignature;

		pReflector->GetInputParameterDesc(i, &parameterSignature);

		inputElement.SemanticName = parameterSignature.SemanticName;
		inputElement.SemanticIndex = parameterSignature.SemanticIndex;
		inputElement.InputSlot = i; // TODO: If using interleaved arrays, then the input slot should be 0.  If using packed arrays, the input slot will vary.
		inputElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // TODO: Figure out how to deal with per-instance data? .. Don't. Just use structured buffers to store per-instance data and use the SV_InstanceID as an index in the structured buffer.
		inputElement.InstanceDataStepRate = 0;
		inputElement.Format = GetDXGIFormat(parameterSignature);

		assert(inputElement.Format != DXGI_FORMAT_UNKNOWN);

		inputElements.push_back(inputElement);

		m_InputSemantics.insert(SemanticMap::value_type(BufferBinding(inputElement.SemanticName, inputElement.SemanticIndex), i));
	}

	if (inputElements.size() > 0)
	{
		hr = m_pDevice->CreateInputLayout(inputElements.data(), (UINT)inputElements.size(), m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), &m_pInputLayout);
		if (FAILED(hr))
		{
			Log::Error("Failed to create input layout.");
			return false;
		}
	}*/

	// Query Resources that are bound to the shader.
	/*for (UINT i = 0; i < shaderDescription.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		pReflector->GetResourceBindingDesc(i, &bindDesc);
		std::string resourceName = bindDesc.Name;

		ShaderParameter::Type parameterType = ShaderParameter::Type::Invalid;

		switch (bindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
			parameterType = ShaderParameter::Type::Texture;
			break;
		case D3D_SIT_SAMPLER:
			parameterType = ShaderParameter::Type::Sampler;
			break;
		case D3D_SIT_CBUFFER:
		case D3D_SIT_STRUCTURED:
			parameterType = ShaderParameter::Type::Buffer;
			break;
		case D3D_SIT_UAV_RWSTRUCTURED:
			parameterType = ShaderParameter::Type::RWBuffer;
			break;
		case D3D_SIT_UAV_RWTYPED:
			parameterType = ShaderParameter::Type::RWTexture;
			break;
		}

		// Create an empty shader parameter that should be filled-in by the application.
		std::shared_ptr<ShaderParameterOGL> shaderParameter = std::make_shared<ShaderParameterOGL>(resourceName, bindDesc.BindPoint, shaderType, parameterType);
		m_ShaderParameters.insert(ParameterMap::value_type(resourceName, shaderParameter));

	}*/

	// Now try to restore the original shader parameters (if there were any)
	/*for (auto shaderParameter : shaderParameters)
	{
		ParameterMap::iterator iter = m_ShaderParameters.find(shaderParameter.first);
		if (iter != m_ShaderParameters.end())
		{
			iter->second = shaderParameter.second;
		}
	}*/

	return true;
}

bool ShaderOGL::LoadShaderFromFile(ShaderType shaderType, cstring fileName, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile)
{
	bool result = false;

	{
		std::shared_ptr<IFile> file = GetManager<IFilesManager>()->Open(fileName);

		string data = "";
		while (!file->isEof())
		{
			string line;
			file->readLine(&line);

			data += line + '\n';
		}

		result = LoadShaderFromString(shaderType, data, fileName, shaderMacros, entryPoint, profile);
	}

	return result;
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
