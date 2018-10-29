#pragma once

class ADT_CHUNK_Material : public Material
{
public:
	ADT_CHUNK_Material(RenderDevice* _device);
	virtual ~ADT_CHUNK_Material();

	uint32 GetLayersCnt() const;
	void SetLayersCnt(uint32 value);

protected:
	void UpdateConstantBuffer() const;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: m_DiffuseColor(1, 1, 1, 1)
		{}
		vec4   m_DiffuseColor;
		uint32 m_LayersCnt;
		vec3   m_Pad;
		//-------------------------- ( 32 bytes )
	};
	MaterialProperties* m_pProperties;
};