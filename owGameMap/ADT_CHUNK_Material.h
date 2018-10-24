#pragma once

class ADT_CHUNK_Material : public Material
{
public:
	ADT_CHUNK_Material(RenderDevice* _device);
	virtual ~ADT_CHUNK_Material();

	std::shared_ptr<Texture> GetTexture(uint8 ID) const;
	void SetTexture(uint8 type, std::shared_ptr<Texture> texture);

	uint32 GetLayersCnt() const;
	void SetLayersCnt(uint32 value);

protected:
	virtual TextureMap GetTextureMap() const;
	std::shared_ptr<ConstantBuffer> GetConstantBuffer() const override;
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
	std::shared_ptr<ConstantBuffer> m_pConstantBuffer;

	// Textures
	TextureMap m_Textures;
};