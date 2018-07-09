#pragma once

#include __PACK_BEGIN
struct Liquid_Layer
{
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

	void Render(cmat4 _worldMatrix);

protected:
	void initGeometry(DBC_LIQUIDTYPE_Type _type, IFile* f);
	void createBuffer();
	void InitTextures(DBC_LIQUIDTYPE_Type _liquidType);

protected:
	DBC_LIQUIDTYPE_Type			m_Type;
	uint32						m_TilesX, m_TilesY;
	uint32						m_TilesCount;

	SmartGeomPtr				__geom;
	uint32						m_VerticesCnt;
	uint32						m_IndicesCnt;

	vector<SmartTexturePtr>		m_Textures;

	Liquid_Layer layer;

	float						ydir;
	uint8						flag;

private:
	ISkyManager*				m_SkyManager;

private:
	const vec3 defaultNormal = vec3(0.0f, 1.0f, 0.0f);
};
