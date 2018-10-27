#pragma once

#include __PACK_BEGIN

struct MH2O_Header
{
	uint32 offsetInstances; // This is an Offset to the first Water Layer, it has tobe an offset because there can be multiple layers.
	uint32 layersCount;    // 0 if the chunk has no liquids. If > 1, the offsets will point to arrays.

						   /*
						   This field requires some explanation.
						   Its an offset to a uint8[8] data block.
						   the data block contains a bit field 8x8 = 64 bit every bit represents a quad between 4 verts.
						   But the bits in this field are only set to 1 if the field is really visible this is not ment in a
						   technical way. To make it clear all of the 4 vertices have an alpha value (i explain them later)
						   if only one of these alpha values smaller then 10 the bit is not set

						   if the first byte is 0x17h which is in binary 00010111b it has to look like this in a grid:

						   0123...
						   1+-----> bits
						   2| 11101000
						   3|
						   .|
						   .V
						   bytes

						   */
	uint32 offsetAttributes;
};

struct Liquid_Layer
{
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

	std::shared_ptr<IMesh>                    m_Mesh;
	std::vector<std::shared_ptr<Texture>>    m_Textures;
};
#include __PACK_END

class Liquid
{
public:
	Liquid(uint32 x, uint32 y);

	void Render(cmat4 _worldMatrix);

protected:
	void initGeometry(const DBC_LiquidTypeRecord* _type, IFile* f);
	void createBuffer();
	

protected:
	uint32						m_TilesX, m_TilesY;
	uint32						m_TilesCount;
	BoundingBox					m_Bounds;

	std::vector<Liquid_Layer>	m_WaterLayers;

	float						ydir;

private:
	ISkyManager*				m_SkyManager;
	CGroupQuality&				m_QualitySettings;

private:
	const vec3 defaultNormal = vec3(0.0f, 1.0f, 0.0f);
};
