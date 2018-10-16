#pragma once

#include "Material.h"
class Texture;
class ConstantBuffer;

class MaterialDebug : public Material
{
public:
	MaterialDebug(RenderDevice* renderDevice);
	virtual ~MaterialDebug();

	virtual void Bind(std::weak_ptr<Shader> pShader) const override;

	cvec4 GetDiffuseColor() const;
	void SetDiffuseColor(cvec4 diffuse);

private:
	// If the material properties have changed, update the contents of the constant buffer.
	void UpdateConstantBuffer();

	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: m_DiffuseColor(1, 1, 1, 1)
		{}
		vec4   m_DiffuseColor;
		//-------------------------- ( 16 bytes )
	};

	// Material properties have to be 16 byte aligned.
	// To guarantee alignment, we'll use _aligned_malloc to allocate memory
	// for the material properties.
	MaterialProperties* m_pProperties;

	// Constant buffer that stores material properties.
	// This material owns this constant buffer and will delete it 
	// when the material is destroyed.
	std::shared_ptr<ConstantBuffer> m_pConstantBuffer;
};