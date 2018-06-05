#pragma once

#include __PACK_BEGIN
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
	vector<pair<float, float>> textureCoords;

	vector<bool> renderTiles;
};
#include __PACK_END

class Liquid : public CRefItem
{
public:
	Liquid(uint32 x, uint32 y);

	uint32 getVerticesCnt() const { return m_VerticesCnt; }


	void Render();

protected:
	void initGeometry(IFile* f);
	void createBuffer();
	void InitTextures(DBC_LIQUIDTYPE_Type _liquidType);

protected:
	uint32						m_TilesX, m_TilesY;
	uint32						m_TilesCount;

	SmartGeomPtr				__geom;
	uint32						m_VerticesCnt;

	vector<SmartTexturePtr>		textures;

	vector<Liquid_Layer>		m_WaterLayers;

	float						ydir;

private:
	ISkyManager*				m_SkyManager;

private:
	const vec3 defaultNormal = vec3(0.0f, 1.0f, 0.0f);
};
