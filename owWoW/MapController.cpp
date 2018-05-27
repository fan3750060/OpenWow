#include "stdafx.h"

// General
#include "MapController.h"

// Additional
#include "WorldController.h"
#include "Map_Shared.h"

MapController::MapController()
{
    memset(m_ADTCache, 0, sizeof(m_ADTCache));
    m_CurrentTileX = m_CurrentTileZ = -1;
    memset(m_Current, 0, sizeof(m_Current));
    m_IsOnInvalidTile = false;

    Map_Shared::CreateMapArrays();

    ADDCONSOLECOMMAND_CLASS("map_clear", MapController, ClearCache);

	_Bindings->RegisterUpdatableObject(this);
}

MapController::~MapController()
{
	_Bindings->UnregisterUpdatableObject(this);
}

//

void MapController::InitWMOs()
{
    // Load global WMO

	//!!!!!!!!!!!!!!!!!!!!!
}

//

void MapController::PreLoad(DBC_MapRecord* _map)
{
    m_DBC_Map = _map;

    Log::Print("Map[%s]: Id [%d]. Preloading...", m_DBC_Map->Get_Directory(), m_DBC_Map->Get_ID());

    m_MapFolder = "World\\Maps\\" + string(m_DBC_Map->Get_Directory()) + "\\";


	m_WDL.Load(m_MapFolder + m_DBC_Map->Get_Directory() + ".wdl");
}

void MapController::Load()
{
	Log::Print("Map[%s]: Id [%d]. Loading...", m_DBC_Map->Get_Directory(), m_DBC_Map->Get_ID());

	_World->EnvM()->InitSkies(m_DBC_Map);

	m_WDT.Load(m_MapFolder + m_DBC_Map->Get_Directory() + ".wdt");
}

void MapController::PostLoad()
{
	Log::Print("Map[%s]: Id [%d]. Postloading...", m_DBC_Map->Get_Directory(), m_DBC_Map->Get_ID());

	m_WDT.InitGlobalWMO();
	m_WDL.InitLowResolutionWMOs();
}

void MapController::Unload()
{
    for (int i = 0; i < C_TilesCacheSize; i++)
    {
        if (m_ADTCache[i] != nullptr)
        {
            delete m_ADTCache[i];
        }
    }
}

//

void MapController::Update(double _Time, double _deltaTime)
{
    bool loading = false;
    int enteredTileX, enteredTileZ;
    int midTile = static_cast<uint32>(C_RenderedTiles / 2);
    if (m_Current[midTile][midTile] != nullptr || m_IsOnInvalidTile)
    {
        if (m_IsOnInvalidTile ||
            (_Camera->Position.x < m_Current[midTile][midTile]->getTranslate().x) ||
            (_Camera->Position.x > (m_Current[midTile][midTile]->getTranslate().x + C_TileSize)) ||
            (_Camera->Position.z < m_Current[midTile][midTile]->getTranslate().z) ||
            (_Camera->Position.z > (m_Current[midTile][midTile]->getTranslate().z + C_TileSize)))
        {

            enteredTileX = static_cast<int>(_Camera->Position.x / C_TileSize);
            enteredTileZ = static_cast<int>(_Camera->Position.z / C_TileSize);

            loading = true;
        }
    }

    if (loading)
    {
        if (enteredTileX != -1 && enteredTileZ != -1)
        {
            EnterMap(enteredTileX, enteredTileZ);
            enteredTileX = enteredTileZ = -1;
            loading = false;
        }
    }
}

//

/*void MapController::RenderTiles()
{
    // Draw cache
    for (int i = 0; i < C_TilesCacheSize; i++)
    {
        if (m_ADTCache[i] != nullptr)
        {
            m_ADTCache[i]->draw();
        }
    }

    // Draw current
    for (int i = 0; i < C_RenderedTiles; i++)
        for (int j = 0; j < C_RenderedTiles; j++)
            if (current[i][j] != nullptr)
                current[i][j]->draw();
}*/

//

void MapController::EnterMap(int32 x, int32 z)
{
    if (IsBadTileIndex(x, z) || !m_WDT.m_TileFlag[x][z].flags.Flag_HasADT)
    {
        m_IsOnInvalidTile = true;
        return;
    }

    m_CurrentTileX = x;
    m_CurrentTileZ = z;

    for (uint8 i = 0; i < C_RenderedTiles; i++)
    {
        for (uint8 j = 0; j < C_RenderedTiles; j++)
        {
            m_Current[i][j] = LoadTile(x - static_cast<uint32>(C_RenderedTiles / 2) + i, z - static_cast<uint32>(C_RenderedTiles / 2) + j);
        }
    }
}

ADT* MapController::LoadTile(int32 x, int32 z)
{
    if (IsBadTileIndex(x, z))
    {
        return nullptr;
    }

    if (!m_WDT.m_TileFlag[x][z].flags.Flag_HasADT)
    {
        return nullptr;
    }

    // Try get tile from cache
    int firstnull = C_TilesCacheSize;
    for (int i = 0; i < C_TilesCacheSize; i++)
    {
        if ((m_ADTCache[i] != nullptr) && (m_ADTCache[i]->m_IndexX == x) && (m_ADTCache[i]->m_IndexZ == z))
        {
            return m_ADTCache[i];
        }

        if ((m_ADTCache[i] == nullptr) && (i < firstnull))
        {
            firstnull = i;
        }
    }

    // ok we need to find a place in the cache
    if (firstnull == C_TilesCacheSize)
    {
        int score, maxscore = 0, maxidx = 0;
        // oh shit we need to throw away a tile
        for (int i = 0; i < C_TilesCacheSize; i++)
        {
            if (m_ADTCache[i] == nullptr)
            {
                continue;
            }

            score = abs(m_ADTCache[i]->m_IndexX - m_CurrentTileX) + abs(m_ADTCache[i]->m_IndexZ - m_CurrentTileZ);

            if (score > maxscore)
            {
                maxscore = score;
                maxidx = i;
            }
        }

        // maxidx is the winner (loser)
        delete m_ADTCache[maxidx];
        m_ADTCache[maxidx] = nullptr;
        firstnull = maxidx;
    }

    // Create new tile
    m_ADTCache[firstnull] = new ADT(x, z);
	m_ADTCache[firstnull]->Load(m_DBC_Map->Get_Directory());
    return m_ADTCache[firstnull];
}

void MapController::ClearCache()
{
    for (int i = 0; i < C_TilesCacheSize; i++)
    {
        if (m_ADTCache[i] != nullptr && !IsTileInCurrent(m_ADTCache[i]))
        {
            delete m_ADTCache[i];
            m_ADTCache[i] = nullptr;
        }
    }
}

uint32 MapController::GetAreaID()
{
    int tileX, tileZ, chunkX, chunkZ;
    tileX = (int)(_Camera->Position.x / C_TileSize);
    tileZ = (int)(_Camera->Position.z / C_TileSize);

    chunkX = (int)(fmod(_Camera->Position.x, C_TileSize) / C_ChunkSize);
    chunkZ = (int)(fmod(_Camera->Position.z, C_TileSize) / C_ChunkSize);

	if (
		(tileX < m_CurrentTileX - static_cast<int>(C_RenderedTiles / 2)) || 
		(tileX > m_CurrentTileX + static_cast<int>(C_RenderedTiles / 2)) ||
		(tileZ < m_CurrentTileZ - static_cast<int>(C_RenderedTiles / 2)) || 
		(tileZ > m_CurrentTileZ + static_cast<int>(C_RenderedTiles / 2))
		)
	{
		return UINT32_MAX;
	}

	ADT* curTile = m_Current[tileZ - m_CurrentTileZ + static_cast<int>(C_RenderedTiles / 2)][tileX - m_CurrentTileX + static_cast<int>(C_RenderedTiles / 2)];
	if (curTile == nullptr)
	{
		return UINT32_MAX;
	}

    ADT_MCNK* curChunk = curTile->getChunk(chunkX, chunkZ);
	if (curChunk == nullptr)
	{
		return UINT32_MAX;
	}

    return curChunk->header.areaid;
}

bool MapController::IsTileInCurrent(ADT* _mapTile)
{
    for (int i = 0; i < C_RenderedTiles; i++)
        for (int j = 0; j < C_RenderedTiles; j++)
            if (m_Current[i][j] != nullptr)
                if (m_Current[i][j] == _mapTile)
                    return true;
    return false;
}
