#pragma once

#include "Shader.h"
#include "Texture.h"


// A material class is used to wrap the shaders and to manage the shader parameters.
class Material : public Object
{
public:
	typedef std::map<Shader::ShaderType, std::shared_ptr<Shader> > ShaderMap;
	typedef std::map<uint8, std::shared_ptr<Texture> > TextureMap;

public:
	virtual void SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader) = 0;
	virtual std::shared_ptr<Shader> GetShader(Shader::ShaderType type) const = 0;
	virtual const ShaderMap& GetShaders() const = 0;

	virtual std::shared_ptr<Texture> GetTexture(uint8 ID) const = 0;
	virtual void SetTexture(uint8 type, std::shared_ptr<Texture> texture) = 0;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void CreateConstantBuffer(void* _data) = 0;
	virtual void UpdateConstantBuffer() const = 0;
	virtual void UpdateConstantBuffer(void* _data) const = 0;
	virtual void MarkConstantBufferDirty() = 0;
};