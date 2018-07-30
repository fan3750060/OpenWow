#include "stdafx.h"

// General
#include "MapController.h"

MapController::MapController() :
	m_WDT(nullptr),
	m_WDL(nullptr),
	m_SkyManager(nullptr),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
    memset(m_ADTCache, 0, sizeof(m_ADTCache));
    m_CurrentTileX = m_CurrentTileZ = -1;
    memset(m_Current, 0, sizeof(m_Current));
    m_IsOnInvalidTile = false;

	if (_MapShared == nullptr)
	{
		_MapShared = new CMapShared();
	}

    ADDCONSOLECOMMAND_CLASS("map_clear", MapController, ClearCache);

	AddManager<IMapManager>(this);

	// Scene node
	{
		setOpaque(true);

		BoundingBox bbox(vec3(Math::MinFloat, Math::MinFloat, Math::MinFloat), vec3(Math::MaxFloat, Math::MaxFloat, Math::MaxFloat));
		setBounds(bbox);
	}

	/*time_t t = time(0);   // get time now
	tm* now = localtime(&t);
	m_GameTime.Set(now->tm_hour, now->tm_min);*/
	m_GameTime.Set(11, 0);

	mProvider = nullptr;
	dir = nullptr;

	/*mProvider = new CMinimapProvider;
	mProvider->Init();

	dir = mProvider->getMinimap("Kalimdor");
	dir->Load();*/
}

MapController::~MapController()
{
	SafeDelete(m_SkyManager);
	SafeDelete(_MapShared);

	DelManager<IMapManager>();
}

// --

void MapController::MapPreLoad(const DBC_MapRecord& _map)
{
    m_DBC_Map = _map;
	m_MapFilenameT = CMapShared::getMapFolder(m_DBC_Map);

    Log::Print("Map[%s]: Id [%d]. Preloading...", m_DBC_Map.Get_Directory(), m_DBC_Map.Get_ID());

	SafeDelete(m_WDL);
	m_WDL = new WDL(this);
	m_WDL->Load();

	// Delete if exists
	SafeDelete(m_WDT);
	m_WDT = new WDT(this);
}

void MapController::MapLoad()
{
	Log::Print("Map[%s]: Id [%d]. Loading...", m_DBC_Map.Get_Directory(), m_DBC_Map.Get_ID());

	SafeDelete(m_SkyManager);
	m_SkyManager = new SkyManager(this, m_DBC_Map);

	// Load data
	m_WDT->Load();
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

	for (int i = 0; i < C_RenderedTiles; i++)
	{
		for (int j = 0; j < C_RenderedTiles; j++)
		{
			m_Current[i][j] = nullptr;
		}
	}
}

// --

void MapController::Update()
{
	if (m_QualitySettings.timeEnable)
	{
		m_GameTime.Tick();
	}

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

//--

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
    m_ADTCache[firstnull] = new ADT(this, x, z);
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

	int32 tileX = (int)(_Render->getCamera()->Position.x / C_TileSize);
	int32 tileZ = (int)(_Render->getCamera()->Position.z / C_TileSize);

	int32 chunkX = (int)(fmod(_Render->getCamera()->Position.x, C_TileSize) / C_ChunkSize);
	int32 chunkZ = (int)(fmod(_Render->getCamera()->Position.z, C_TileSize) / C_ChunkSize);

	if (
		(tileX < m_CurrentTileX - static_cast<int32>(C_RenderedTiles / 2)) || 
		(tileX > m_CurrentTileX + static_cast<int32>(C_RenderedTiles / 2)) ||
		(tileZ < m_CurrentTileZ - static_cast<int32>(C_RenderedTiles / 2)) || 
		(tileZ > m_CurrentTileZ + static_cast<int32>(C_RenderedTiles / 2))
		)
	{
		return UINT32_MAX;
	}

	int32 indexX = tileZ - m_CurrentTileZ + static_cast<int32>(C_RenderedTiles / 2);
	int32 indexY = tileX - m_CurrentTileX + static_cast<int32>(C_RenderedTiles / 2);

	ADT* curTile = m_Current[indexX][indexY];
	if (curTile == nullptr)
	{
		return UINT32_MAX;
	}

	std::shared_ptr<ADT_MCNK> curChunk = curTile->getChunk(chunkX, chunkZ);
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
