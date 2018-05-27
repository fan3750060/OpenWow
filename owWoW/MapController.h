#pragma once

// Includes
#include "ADT.h"
#include "WDT.h"
#include "WDL.h"


class MapController : public IUpdatable
{
public:
	MapController();
	~MapController();

	void PreLoad(DBC_MapRecord* _map);
	void Load();
	void PostLoad();

	void Unload();

	//

	void EnterMap(int32 x, int32 z);
	ADT* LoadTile(int32 x, int32 z);
	void ClearCache();
	uint32 GetAreaID();

	// IUpdatable
	void Input(double _time, double _dTime) override {}
	void Update(double _Time, double _deltaTime) override;

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

inline bool IsBadTileIndex(int i, int j)
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

inline bool IsGoodTileIndex(int i, int j)
{
	return (!IsBadTileIndex(i, j));
}