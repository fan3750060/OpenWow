#pragma once

#include "Material.h"
class ConstantBuffer;

class MaterialDebug : public Material
{
public:
	MaterialDebug(RenderDevice* renderDevice);
	virtual ~MaterialDebug();

	cvec4 GetDiffuseColor() const;
	void SetDiffuseColor(cvec4 diffuse);

private:
	void UpdateConstantBuffer() const;

	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: m_DiffuseColor(1, 1, 1, 1)
		{}
		vec4   m_DiffuseColor;
		//-------------------------- ( 16 bytes )
	};
	MaterialProperties* m_pProperties;
};