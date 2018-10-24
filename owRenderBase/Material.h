#pragma once

class RenderDevice;
class Shader;

// A material class is used to wrap the shaders and to manage the shader parameters.
class Material : public Object
{
public:
	typedef std::map<Shader::ShaderType, std::shared_ptr<Shader> > ShaderMap;
	typedef std::map<uint8, std::shared_ptr<Texture> > TextureMap;

public:
	Material(RenderDevice* renderDevice);
	virtual ~Material();

	virtual void SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader);
	virtual std::shared_ptr<Shader> GetShader(Shader::ShaderType type) const;
	virtual const ShaderMap& GetShaders() const;

	virtual void Bind();
	virtual void Unbind();

protected:
	virtual TextureMap GetTextureMap() const;
	virtual std::shared_ptr<ConstantBuffer> GetConstantBuffer() const;
	virtual void UpdateConstantBuffer() const;

protected:
	ShaderMap     m_Shaders;

	RenderDevice* m_RenderDevice;
	bool          m_Dirty;
};