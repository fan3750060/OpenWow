#pragma once

class ShaderParameterOGL;

class ShaderOGL : public Shader
{
public:
	ShaderOGL();
	virtual ~ShaderOGL();

	virtual ShaderType GetType() const;

	// Shader loading
	//virtual bool LoadShaderFromString(ShaderType type, cstring source, cstring sourceFileName, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile);
	virtual bool LoadShaderFromFile(ShaderType type, cstring fileName, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile);

	//virtual UINT GetConstantBufferIndex( cstring name );
	virtual ShaderParameter& GetShaderParameterByName(cstring name) const;

	//virtual ConstantBuffer* GetConstantBufferByName( cstring name ); 

	// Check to see if this shader supports a given semantic.
	bool HasSemantic(const BufferBinding& binding) const;
	UINT GetSlotIDBySemantic(const BufferBinding& binding) const;

	virtual void Bind();
	virtual void UnBind();

	virtual void Dispatch(const glm::uvec3& numGroups);

public:
	uint32 GetGLObject();

protected:
	bool GetShaderProgramLog(uint32 _obj, std::string * _errMsg);
	// Destroy the contents of this shader (in case we are loading a new shader).
	virtual void Destroy();

private:
	uint32 m_GLShaderProgramm;

private:
	ShaderType	m_ShaderType;

	typedef std::map<std::string, std::shared_ptr<ShaderParameterOGL> > ParameterMap;
	ParameterMap m_ShaderParameters;

	// A map to convert a vertex attribute semantic to a slot.
	typedef std::map<BufferBinding, UINT> SemanticMap;
	SemanticMap m_InputSemantics;

	// Parameters necessary to reload the shader at runtime if it is modified on disc.
	ShaderMacros m_ShaderMacros;
	std::string m_EntryPoint;
	std::string m_Profile;
	std::string m_ShaderFileName;

	typedef std::unique_lock<std::recursive_mutex> MutexLock;
	std::recursive_mutex m_Mutex;

};
