#pragma once

#include "Map_Chunk.h"

#include "Model_Instance.h"
#include "WMO_Instance.h"

struct ADT_MHDR
{
	uint32 unk0;

	uint32 MCIN;
	uint32 MTEX;
	uint32 MMDX;
	uint32 MMID;
	uint32 MWMO;
	uint32 MWID;
	uint32 MDDF;
	uint32 MODF;

	uint8 unk1[28];
};

struct SMChunkInfo
{
	uint32_t offset;               // absolute offset.
	uint32_t size;                 // the size of the MCNK chunk, this is refering to.
	uint32_t flags;                // always 0. only set in the client., FLAG_LOADED = 1
	union
	{
		uint8_t  unk0[4];
		uint32_t asyncId;
	};
};

struct Map_Tile_TextureInfo
{
    Map_Tile_TextureInfo():
        diffuseTexture(nullptr),
        specularTexture(nullptr)
    {
        mtxf.do_not_load_specular_or_height_texture_but_use_cubemap = false;
    }

    //

    string textureName;
    struct 
    {
        uint32_t do_not_load_specular_or_height_texture_but_use_cubemap : 1;
        uint32_t : 31;
    } mtxf;
    R_Texture* diffuseTexture;
    R_Texture* specularTexture;
};

class MapTile
{
public:
	MapTile(uint32 _intexX, uint32 _intexZ);
	~MapTile();

	//

	bool Load(cstring _filename);

	//

	void draw();
    void Render_DEBUG();
	void drawWater();
	void drawObjects();
	void drawSky();
	void drawModels();

	//

	bool operator==(const MapTile& t) const
	{
		return (t.m_IndexX == m_IndexX) && (t.m_IndexZ == m_IndexZ);
	}
	bool operator!=(const MapTile& t) const
	{
		return (t.m_IndexX != m_IndexX) || (t.m_IndexZ != m_IndexZ);
	}

	MapChunk* getChunk(uint32 x, uint32 z)
    {
        assert1(x < C_ChunksInTile && z < C_ChunksInTile);
        return m_Chunks[x * C_ChunksInTile + z];
    }

public:
	ADT_MHDR					 m_Header;
    int                          m_IndexX, m_IndexZ;
    float                        m_GamePositionX, m_GamePositionZ;

	vector<Map_Tile_TextureInfo> m_Textures;

	vector<string>               m_WMOsNames;
    vector<WMOPlacementInfo>     m_WMOsPlacementInfo;
	vector<WMOInstance*>         m_WMOsInstances;

	vector<string>               m_MDXsNames;
    vector<ModelPlacementInfo>   m_MDXsPlacementInfo;
	vector<ModelInstance*>       m_MDXsInstances;

    vector<MapChunk*>            m_Chunks;
    vector<Liquid*>              m_MH2O;
};





