#pragma once

#include "ADT.h"
#include "WDT.h"
#include "WDL.h"

class MapController : public SceneNode, public IMapManager
{
public:
	MapController();
	~MapController();

	void MapPreLoad(DBC_MapRecord* _map);
	void MapLoad();
	void MapPostLoad();

	void Unload();

	//

	void EnterMap(int32 x, int32 z);
	ADT* LoadTile(int32 x, int32 z);
	void ClearCache();
	uint32 GetAreaID();

	// IRenderable3D
	void PreRender3D() override;

	// IMapManager
	void Update() override;

public: // Getters
	string GetFolder() { return m_MapFolder; }
	DBC_MapRecord* GetDBCMap() { return m_DBC_Map; }

	int GetCurrentX() { return m_CurrentTileX; }
	int GetCurrentZ() { return m_CurrentTileZ; }

	bool IsOutOfBounds() const { return m_IsOnInvalidTile; }
	void SetOutOfBounds(bool _value) { m_IsOnInvalidTile = _value; }

private:
	bool IsTileInCurrent(ADT* _mapTile);

private:
	string			m_MapFolder;
	DBC_MapRecord*	m_DBC_Map;


private:
	ADT*	m_ADTCache[C_TilesCacheSize];
	ADT*	m_Current[C_RenderedTiles][C_RenderedTiles];
	
	int32	m_CurrentTileX, m_CurrentTileZ;
	bool	m_IsOnInvalidTile;

public:
	WDT*    m_WDT;
	WDL*    m_WDL;
};

inline static bool IsBadTileIndex(int i, int j)
{
	if (i < 0)
	{
		return true;
	}

	if (j < 0)
	{
		return true;
	}

	if (i >= C_TilesInMap)
	{
		return true;
	}

	if (j >= C_TilesInMap)
	{
		return true;
	}

	return false;
}

inline static bool IsGoodTileIndex(int i, int j)
{
	return (!IsBadTileIndex(i, j));
}