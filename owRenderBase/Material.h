#pragma once

class RenderDevice;
class Shader;

// A material class is used to wrap the shaders and to manage the shader parameters.
class Material : public Object
{
public:
	Material(RenderDevice* renderDevice);
	virtual ~Material();

	virtual void Bind(std::weak_ptr<Shader> pShader) const = 0;

protected:
	RenderDevice* m_RenderDevice;
	bool          m_Dirty;
};