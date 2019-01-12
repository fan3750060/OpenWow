#pragma once

class Liquid_Layer : public MeshWrapper
{
public:
	Liquid_Layer(std::shared_ptr<IMesh> _mesh);
	virtual ~Liquid_Layer();



public:
	const DBC_LiquidTypeRecord* LiquidType;
	uint16 VertexFormat;

	float MinHeightLevel;
	float MaxHeightLevel;

	uint8 x;
	uint8 y;
	uint8 Width;
	uint8 Height;

	bool hasmask;
	uint8 mask[8];

	std::vector<float> heights;
	std::vector<uint8> depths;
	std::vector<std::pair<float, float>> textureCoords;

	std::vector<bool> renderTiles;

	// Render
	void InitTextures();

	std::vector<std::shared_ptr<Texture>>    m_Textures;
};

class Liquid
{
public:
	Liquid(uint32 x, uint32 y);

protected:
	void createLayers(const DBC_LiquidTypeRecord* _type, std::shared_ptr<IFile> f);
	void createBuffer();
	
public:
	uint32                                                              m_TilesX, m_TilesY;
	uint32                                                              m_TilesCount;
	BoundingBox                                                         m_Bounds;

	std::vector<std::shared_ptr<Liquid_Layer>>							m_WaterLayers;

	float                                                               ydir;

private:
	ISkyManager*                                                        m_SkyManager;
	const CGroupQuality&                                                m_QualitySettings;

private:
	const vec3 defaultNormal = vec3(0.0f, 1.0f, 0.0f);
};
