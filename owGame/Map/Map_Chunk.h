#pragma once

// Includes and uses
#include "../Liquid.h"
#include "Map_Chunk_Types.h"
class MapTile;

class MapChunk
{
public:
	MapChunk(MapTile* _parentTile);
	~MapChunk();

	void init(File& f, load_phases _phase);

	void initStrip(int holes);

	void Render();
	void drawNoDetail();
	void drawPass(int anim);

	//

	void CreateMH2OLiquid(File& f, MH2O_Header* _liquidHeader);
	MCNK_MCLQ_LiquidType GetLiquidType();

public:
	MapTile* m_ParentTile;
	float m_GamePositionX, m_GamePositionY, m_GamePositionZ;

	int areaID;

	bool visible;
	bool hasholes;
	float waterlevel[2];

	Texture* textures[4];
	Texture* SpecularTextures[4];
	Texture* blend;

	int animated[4];



	GLuint globalBuffer;

	short* strip;
	int striplen;

	Liquid* lq;

	BoundingBox m_Bounds;

private:
	MCNK_Header* header;
	bool MCLV_exists;
	bool colorBufferEnable;
};