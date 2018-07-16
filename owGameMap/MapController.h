#pragma once

#include "Map_Shared.h"
#include "SkyManager.h"
#include "ADT.h"
#include "WDT.h"
#include "WDL.h"
#include "MinimapProvider.h"

class MapController : public SceneNode, public IMapManager
{
public:
	MapController();
	virtual ~MapController();

	void MapPreLoad(const DBC_MapRecord& _map);
	void MapLoad();
	void MapPostLoad();
	void Unload();

	//
	void EnterMap(int32 x, int32 z);
	ADT* LoadTile(int32 x, int32 z);
	void ClearCache();
	uint32 GetAreaID();

	// ISceneNode
	string getObjectInfo() const override { return "@MapController@"; }

	// IMapManager
	void Update() override;

public: // Getters
	string getFilenameT() const { return m_MapFilenameT; }
	const DBC_MapRecord& GetDBCMap() const { return m_DBC_Map; }

	bool isUncompressedAlpha() const { return m_WDT->getFlags()->Flag_8bitMCAL; }
	bool isTileBased() const { return m_WDT->MapHasTiles(); }
	const ADT_WMO_Instance* getGlobalInstance() const { return m_WDT->GetGlobalWMOInstance(); }

	int GetCurrentX() const { return m_CurrentTileX; }
	int GetCurrentZ() const { return m_CurrentTileZ; }

	void SetOutOfBounds(bool _value) { m_IsOnInvalidTile = _value; }
	bool IsOutOfBounds() const { return m_IsOnInvalidTile; }
	
	R_Texture* getMinimap() const { return m_WDL->getMinimap(); }
	const SkyManager* getSkyManager() const { return m_SkyManager; }
	const WowTime* getTime() const { return &m_GameTime; }

	bool getTileIsCurrent(int x, int z) const
	{
		int midTile = static_cast<uint32>(C_RenderedTiles / 2);
		ADT* currentTile = m_Current[midTile][midTile];
		if (currentTile == nullptr)
		{
			return false;
		}
	
		int32 currentX = currentTile->m_IndexX;
		int32 currentZ = currentTile->m_IndexZ;

		return (
				x >= (currentX - (C_RenderedTiles / 2)) &&
				z >= (currentZ - (C_RenderedTiles / 2)) &&
				x <= (currentX + (C_RenderedTiles / 2)) &&
				z <= (currentZ + (C_RenderedTiles / 2))
				);
	}

private:
	bool IsTileInCurrent(ADT* _mapTile);

private:
	string					m_MapFilenameT;
	DBC_MapRecord			m_DBC_Map;

	ADT*					m_ADTCache[C_TilesCacheSize];
	ADT*					m_Current[C_RenderedTiles][C_RenderedTiles];
	int32					m_CurrentTileX, m_CurrentTileZ;
	bool					m_IsOnInvalidTile;

	WDT*					m_WDT;
	WDL*					m_WDL;
	SkyManager*				m_SkyManager;
	WowTime					m_GameTime;

	// Minimap
	CMinimapProvider*		mProvider;
	MinimapDir*				dir;

private: // PARENT
	CGroupQuality& m_QualitySettings;
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