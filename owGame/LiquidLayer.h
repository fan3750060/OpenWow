#pragma once

#include "LiquidMaterial.h"

#include "DBC\\Core\DBC__Storage.h"

class Liquid_Layer : public MeshWrapper
{
public:
	Liquid_Layer(std::shared_ptr<IMesh> _mesh);
	virtual ~Liquid_Layer();

	// IMesh
	bool Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) override;

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

	std::shared_ptr<LiquidMaterial> m_Material;
	std::vector<std::shared_ptr<Texture>>    m_Textures;

private:
	std::weak_ptr<ISkyManager>                                          m_SkyManager;
	const CGroupQuality&                                                m_QualitySettings;
};