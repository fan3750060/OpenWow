#pragma once

class IRenderDevice;
class Shader;

// A material class is used to wrap the shaders and to manage the shader parameters.
class Material : public Object
{
public:
	typedef std::map<Shader::ShaderType, std::shared_ptr<Shader> > ShaderMap;
	typedef std::map<uint8, std::shared_ptr<Texture> > TextureMap;

public:
	Material(IRenderDevice* renderDevice);
	virtual ~Material();

	virtual void SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader);
	virtual std::shared_ptr<Shader> GetShader(Shader::ShaderType type) const;
	virtual const ShaderMap& GetShaders() const;

	virtual std::shared_ptr<Texture> GetTexture(uint8 ID) const;
	virtual void SetTexture(uint8 type, std::shared_ptr<Texture> texture);

	virtual void Bind() const;
	virtual void Unbind() const;

protected:
	virtual void CreateConstantBuffer();
	virtual void UpdateConstantBuffer() const;

protected:
	ShaderMap                        m_Shaders;

	TextureMap                       m_Textures;
	std::shared_ptr<ConstantBuffer>  m_pConstantBuffer;

	IRenderDevice*                    m_RenderDevice;
	mutable bool                     m_Dirty;
};