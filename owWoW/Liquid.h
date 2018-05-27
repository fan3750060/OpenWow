#pragma once


#include "../shared/pack_begin.h"

struct Liquid_Layer
{
	float MinHeightLevel;
	float MaxHeightLevel;

	uint8 x;
	uint8 y;
	uint8 Width;
	uint8 Height;

	bool hasmask;
	uint8 mask[8];

	vector<float> heights;
	vector<uint8> depths;
	vector<std::pair<float, float>> textureCoords;

	vector<bool> renderTiles;
};

#include "../shared/pack_end.h"

// Includes and using

class WMOMaterial;
class ADT_MCNK_Header;

// Class

class Liquid
{
public:
	Liquid(uint32 x, uint32 y);
	~Liquid();

	void CreateFromMCLQ(IFile* f, ADT_MCNK_Header header);
	void CreateFromWMO(IFile* f, WMOMaterial* _material, const DBC_LiquidTypeRecord* _liquidType, bool _indoor);
	
	void Render();

private:
	void initGeometry(IFile* f);
	void createBuffer();
	void InitTextures(DBC_LIQUIDTYPE_Type _liquidType);

private:
	uint32 m_TilesX, m_TilesY;
	uint32 m_TilesCount;

	vec3 m_WaterColorLight;
	vec3 m_WaterColorDark;

    R_GeometryInfo* __geom;
	uint32 globalBufferSize;

	vector<R_Texture*> textures;

	vector<Liquid_Layer> m_WaterLayers;

	float ydir;

private:
	const vec3 defaultNormal = vec3(0.0f, 1.0f, 0.0f);
};
