#pragma once

class IRenderDevice;

#include "Material.h"

// A material class is used to wrap the shaders and to manage the shader parameters.
class MaterialImpl : public Material
{
public:
	MaterialImpl(IRenderDevice* renderDevice);
	virtual ~MaterialImpl();

	virtual void SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader);
	virtual std::shared_ptr<Shader> GetShader(Shader::ShaderType type) const;
	virtual const ShaderMap& GetShaders() const;

	virtual std::shared_ptr<Texture> GetTexture(uint8 ID) const;
	virtual void SetTexture(uint8 type, std::shared_ptr<Texture> texture);

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual void CreateConstantBuffer(void* _data);
	virtual void UpdateConstantBuffer() const;
	virtual void UpdateConstantBuffer(void* _data) const;
	virtual void MarkConstantBufferDirty();

protected:
	ShaderMap                        m_Shaders;

	TextureMap                       m_Textures;
	std::shared_ptr<ConstantBuffer>  m_pConstantBuffer;

	IRenderDevice*                   m_RenderDevice;
	mutable bool                     m_Dirty;
};