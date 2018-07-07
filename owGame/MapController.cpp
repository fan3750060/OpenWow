#include "stdafx.h"

// General
#include "MapController.h"

// Additional
#include "WorldController.h"
#include "Map_Shared.h"


MapController::MapController() :
	m_WDT(nullptr),
	m_WDL(nullptr)
{
    memset(m_ADTCache, 0, sizeof(m_ADTCache));
    m_CurrentTileX = m_CurrentTileZ = -1;
    memset(m_Current, 0, sizeof(m_Current));
    m_IsOnInvalidTile = false;

	Map_Shared::Init();

    ADDCONSOLECOMMAND_CLASS("map_clear", MapController, ClearCache);

	AddManager<IMapManager>(this);

	// Scene node
	{
		setOpaque(true);

		m_Bounds.setMin(vec3(Math::MinFloat));
		m_Bounds.setMax(vec3(Math::MaxFloat));
		m_Bounds.calculateCenter();
	}

	mProvider = nullptr;
	dir = nullptr;

	/*mProvider = new CMinimapProvider;
	mProvider->Init();

	dir = mProvider->getMinimap("Kalimdor");
	dir->Load();*/
}

MapController::~MapController()
{
	DelManager<IMapManager>();
}

//

void MapController::MapPreLoad(DBC_MapRecord _map)
{
    m_DBC_Map = _map;

    Log::Print("Map[%s]: Id [%d]. Preloading...", m_DBC_Map.Get_Directory(), m_DBC_Map.Get_ID());

    m_MapFolder = "World\\Maps\\" + string(m_DBC_Map.Get_Directory()) + "\\";

	// Delete if exists
	if (m_WDL != nullptr)
	{
		delete m_WDL; m_WDL = nullptr;
	}

	m_WDL = new WDL(m_MapFolder + m_DBC_Map.Get_Directory() + ".wdl");
	m_WDL->Load();

	// Delete if exists
	if (m_WDT != nullptr)
	{
		delete m_WDT;	m_WDT = nullptr;
	}

	m_WDT = new WDT();
	m_WDT->Load(m_MapFolder + m_DBC_Map.Get_Directory() + ".wdt");
}

void MapController::MapLoad()
{
	Log::Print("Map[%s]: Id [%d]. Loading...", m_DBC_Map.Get_Directory(), m_DBC_Map.Get_ID());

	_World->EnvM()->InitSkies(m_DBC_Map);

	// Load data
	m_WDT->Load(m_MapFolder + m_DBC_Map.Get_Directory() + ".wdt");
}

void MapController::MapPostLoad()
{
	Log::Print("Map[%s]: Id [%d]. Postloading...", m_DBC_Map.Get_Directory(), m_DBC_Map.Get_ID());

	// Create all instances
	m_WDT->CreateInsances(this);
	m_WDL->CreateInsances(this);
}

void MapController::Unload()
{
	if (m_WDL) delete m_WDL;
	if (m_WDT) delete m_WDT;

    for (int i = 0; i < C_TilesCacheSize; i++)
    {
        if (m_ADTCache[i] != nullptr)
        {
            delete m_ADTCache[i];
			m_ADTCache[i] = nullptr;
        }
    }
}

//

void MapController::Update()
{
    bool loading = false;
    int enteredTileX, enteredTileZ;
    int midTile = static_cast<uint32>(C_RenderedTiles / 2);
    if (m_Current[midTile][midTile] != nullptr || m_IsOnInvalidTile)
    {
        if (m_IsOnInvalidTile ||
            (_Render->getCamera()->Position.x < m_Current[midTile][midTile]->getTranslate().x) ||
            (_Render->getCamera()->Position.x > (m_Current[midTile][midTile]->getTranslate().x + C_TileSize)) ||
            (_Render->getCamera()->Position.z < m_Current[midTile][midTile]->getTranslate().z) ||
            (_Render->getCamera()->Position.z > (m_Current[midTile][midTile]->getTranslate().z + C_TileSize)))
        {

            enteredTileX = static_cast<int>(_Render->getCamera()->Position.x / C_TileSize);
            enteredTileZ = static_cast<int>(_Render->getCamera()->Position.z / C_TileSize);

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

void MapController::EnterMap(int32 x, int32 z)
{
    if (IsBadTileIndex(x, z) || !m_WDT->m_TileFlag[x][z].flags.Flag_HasADT)
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

    if (!m_WDT->m_TileFlag[x][z].flags.Flag_HasADT)
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
		//GetManager<ILoader>()->AddToDeleteQueue(m_ADTCache[maxidx]);
        delete m_ADTCache[maxidx];
        m_ADTCache[maxidx] = nullptr;
        firstnull = maxidx;
    }

    // Create new tile
	char name[256];
	sprintf_s(name, "World\\Maps\\%s\\%s_%d_%d.adt", m_DBC_Map.Get_Directory(), m_DBC_Map.Get_Directory(), x, z);

    m_ADTCache[firstnull] = new ADT(this, x, z, name, GetManager<IFilesManager>()->Open(name));
	m_ADTCache[firstnull]->Load();
	//GetManager<ILoader>()->AddToLoadQueue(m_ADTCache[firstnull]);
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
	if (!m_WDT->MapHasTiles())
	{
		return UINT32_MAX;
	}

    int tileX, tileZ, chunkX, chunkZ;
    tileX = (int)(_Render->getCamera()->Position.x / C_TileSize);
    tileZ = (int)(_Render->getCamera()->Position.z / C_TileSize);

    chunkX = (int)(fmod(_Render->getCamera()->Position.x, C_TileSize) / C_ChunkSize);
    chunkZ = (int)(fmod(_Render->getCamera()->Position.z, C_TileSize) / C_ChunkSize);

	if (
		(tileX < m_CurrentTileX - static_cast<int>(C_RenderedTiles / 2)) || 
		(tileX > m_CurrentTileX + static_cast<int>(C_RenderedTiles / 2)) ||
		(tileZ < m_CurrentTileZ - static_cast<int>(C_RenderedTiles / 2)) || 
		(tileZ > m_CurrentTileZ + static_cast<int>(C_RenderedTiles / 2))
		)
	{
		return UINT32_MAX;
	}

	int32 indexX = tileZ - m_CurrentTileZ + static_cast<int>(C_RenderedTiles / 2);
	int32 indexY = tileX - m_CurrentTileX + static_cast<int>(C_RenderedTiles / 2);

	ADT* curTile = m_Current[indexX][indexY];
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

bool MapController::PreRender3D()
{
	return true;
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
