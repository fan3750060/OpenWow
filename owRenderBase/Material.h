#pragma once

class RenderDevice;
class Shader;

// A material class is used to wrap the shaders and to manage the shader parameters.
class Material : public Object
{
public:
	Material(RenderDevice* renderDevice);
	virtual ~Material();

	/**
	 * An associated map that associates a shader to a type.
	 */
	/*typedef std::map<Shader::ShaderType, std::shared_ptr<Shader> > ShaderMap;

	virtual void SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader) = 0;
	virtual std::shared_ptr<Shader> GetShader(Shader::ShaderType type) const = 0;
	virtual const ShaderMap& GetShaders() const = 0;*/

	virtual void Bind(std::weak_ptr<Shader> pShader) const = 0;

protected:
	RenderDevice* m_RenderDevice;
	bool          m_Dirty;
};