#pragma once

#include "ADT_MCNK.h"
#include "ADT_MDX_Instance.h"
#include "ADT_WMO_Instance.h"

struct Map_Tile_TextureInfo
{
	Map_Tile_TextureInfo() :
		diffuseTexture(nullptr),
		specularTexture(nullptr)
	{}

	//

	string textureName;
	R_Texture* diffuseTexture;
	R_Texture* specularTexture;
};

class ADT : public Renderable3DObject
{
public:
	ADT(uint32 _intexX, uint32 _intexZ);
	~ADT();

	void Load(cstring _filename);

	void PreRender3D(double t, double dt) {}
	void Render3D() {}
	void PostRender3D() {}

	void drawObjects();
	void drawSky();
	void drawModels();

	//

	inline bool operator==(const ADT& t) const { return (t.m_IndexX == m_IndexX) && (t.m_IndexZ == m_IndexZ); }
	inline bool operator!=(const ADT& t) const { return (t.m_IndexX != m_IndexX) || (t.m_IndexZ != m_IndexZ); }

	inline ADT_MCNK* getChunk(uint32 x, uint32 z)
	{
		assert1(x < C_ChunksInTile && z < C_ChunksInTile);
		return &m_Chunks[x * C_ChunksInTile + z];
	}

public:
	int                         m_IndexX, m_IndexZ;
	float                       m_GamePositionX, m_GamePositionZ;

	vector<Map_Tile_TextureInfo> m_Textures;

	vector<ADT_WMO_Instance>        m_WMOsInstances;
	vector<ADT_MDX_Instance>      m_MDXsInstances;

	vector<ADT_MCNK>           m_Chunks;
};
