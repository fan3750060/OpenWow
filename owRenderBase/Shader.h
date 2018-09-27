#pragma once

class ShaderParameter;
struct BufferBinding;
class ConstantBuffer;
#include "InputSemantic.h"

class Shader : public Object
{
public:
	typedef Object base;

	enum ShaderType
	{
		UnknownShaderType = 0,
		VertexShader,
		TessellationControlShader,      // Hull Shader in DirectX
		TessellationEvaluationShader,   // Domain Shader in DirectX
		GeometryShader,
		PixelShader,
		ComputeShader,
	};

	Shader();

	/**
	 * What type of shader is this?
	 */
	virtual ShaderType GetType() const;

	/**
	 * A shader macro consists of a macro name and a definition.
	 * Use this to pass macro definitions to the shader compiler.
	 */
	typedef std::map< std::string, std::string > ShaderMacros;


	/**
	 * Load a shader file from a string.
	 * @param type: The type of shader to load.
	 * @param source: The Shader source code in string format.
	 * @param sourceFileName: The file path of the original file if it exists. This is used to determine include paths.
	 * @param entryPoint: The name of the entry-point function to be used by this shader.
	 * @param profile: The shader profile to use to compile this shader.
	 * To use the latest supported profile, specify "latest" here.
	 * @return True if the shader was loaded correctly, or False otherwise.
	 */
	virtual bool LoadShaderFromString(ShaderType type, cstring sourceFileName, cstring source, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile) = 0;

	/**
	 * Load a shader from a file.
	 * @param type: The type of shader to load.
	 * @param fileName: The path to the shader file to load.
	 * @param entryPoint: The name of the entry-point function to be used by this shader.
	 * @param profile: The shader profile to use to compile this shader.
	 * To use the latest supported profile, specify "latest" here.
	 * @return True if the shader was loaded correctly, or False otherwise.
	 */
	virtual bool LoadShaderFromFile(ShaderType type, cstring fileName, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile) = 0;

	/**
	 * Get a reference to a parameter defined in the shader.
	 * @param name: The name of the parameter as defined in the shader file.
	 * @return A reference to the ShaderParameter. If the parameter with the specified name
	 * is not found in the shader, then this function will return an invalid shader parameter.
	 * You can check for validity using the ShaderParameter::IsValid method.
	 */
	virtual ShaderParameter& GetShaderParameterByName(cstring name) const;
	
	// Get a parameter defined in the shader by its name by using in index operator.
	virtual ShaderParameter& operator[](cstring name) const
	{
		return GetShaderParameterByName(name);
	}

	// Check to see if this shader supports a given semantic.
	virtual bool                 HasSemantic(cstring binding) const;
	virtual const InputSemantic& GetSemantic(cstring binding) const;
	virtual UINT                 GetSemanticSlot(cstring binding) const;

	/**
	 * Bind this shader for use in the rendering pipeline.
	 */
	virtual void Bind() = 0;

	/**
	 * Unbind the shader from the rendering pipeline.
	 */
	virtual void UnBind() = 0;

	/**
	 * Dispatch a compute shader. If this shader does not refer to a compute
	 * shader, this function does nothing.
	 * TODO: Refactor this into a Command (and CommandBuffer).
	 */
	virtual void Dispatch(const glm::uvec3& numGroups) = 0;

protected:
	ShaderType	m_ShaderType;

	typedef std::map<std::string, std::shared_ptr<ShaderParameter> > ParameterMap;
	ParameterMap m_ShaderParameters;

	// A map to convert a vertex attribute semantic to a slot.
	typedef std::map<InputSemantic, UINT> SemanticMap;
	SemanticMap m_InputSemantics;
};