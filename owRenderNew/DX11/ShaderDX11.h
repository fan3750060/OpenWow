#pragma once

#include <Shader.h>
#include <BufferBinding.h>

class ShaderParameterDX11;

class ShaderDX11 : public Shader
{
public:
	typedef Shader base;

	ShaderDX11(ID3D11Device2* pDevice);
	virtual ~ShaderDX11();

	virtual ShaderType GetType() const;

	// Shader loading
	virtual bool LoadShaderFromString(ShaderType type, cstring source, cstring sourceFileName, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile);
	virtual bool LoadShaderFromFile(ShaderType type, cstring fileName, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile);

	//virtual UINT GetConstantBufferIndex( cstring name );
	virtual ShaderParameter& GetShaderParameterByName(cstring name) const;

	//virtual ConstantBuffer* GetConstantBufferByName( cstring name ); 

	// Query for the latest supported shader profile
	virtual std::string GetLatestProfile(ShaderType type);

	// Check to see if this shader supports a given semantic.
	bool HasSemantic(const BufferBinding& binding) const;
	UINT GetSlotIDBySemantic(const BufferBinding& binding) const;

	virtual void Bind();
	virtual void UnBind();

	virtual void Dispatch(const glm::uvec3& numGroups);

protected:

	// Destroy the contents of this shader (in case we are loading a new shader).
	virtual void Destroy();

private:
	ShaderType	m_ShaderType;
	Microsoft::WRL::ComPtr<ID3D11Device2> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext2> m_pDeviceContext;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11HullShader> m_pHullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader> m_pDomainShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_pComputeShader;

	Microsoft::WRL::ComPtr<ID3DBlob> m_pShaderBlob;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;

	typedef std::map<std::string, std::shared_ptr<ShaderParameterDX11> > ParameterMap;
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
