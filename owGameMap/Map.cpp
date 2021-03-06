#include "stdafx.h"

// General
#include "Map.h"

CMap::CMap() :
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

	setLoaded();
}

CMap::~CMap()
{
	SafeDelete(_MapShared);
}

// --

void CMap::MapPreLoad(std::shared_ptr<DBC_MapRecord> _map)
{
	m_MapDBCRecord = _map;
	m_MapFolderName = CMapShared::getMapFolder(m_MapDBCRecord);

	Log::Print("Map[%s]: Id [%d]. Preloading...", m_MapDBCRecord->Get_Directory(), m_MapDBCRecord->Get_ID());

	m_WDL.reset();
	m_WDL = std::make_shared<CMapWDL>(std::static_pointer_cast<CMap, SceneNode3D>(shared_from_this()));
	m_WDL->Load();

	m_WDT.reset();
	m_WDT = std::make_shared<CMapWDT>(std::static_pointer_cast<CMap, SceneNode3D>(shared_from_this()));
}

void CMap::MapLoad()
{
	Log::Print("Map[%s]: Id [%d]. Loading...", m_MapDBCRecord->Get_Directory(), m_MapDBCRecord->Get_ID());

	DelManager<ISkyManager>();
	m_SkyManager.reset();

	m_SkyManager = CreateSceneNode<SkyManager>();
    m_SkyManager->Load();
	AddManager<ISkyManager>(m_SkyManager);

	m_EnvironmentManager.reset();
	m_EnvironmentManager = std::make_shared<EnvironmentManager>();

	// Load data
	m_WDT->Load();
}

void CMap::MapPostLoad()
{
	Log::Print("Map[%s]: Id [%d]. Postloading...", m_MapDBCRecord->Get_Directory(), m_MapDBCRecord->Get_ID());

	m_WDT->CreateInsances(weak_from_this());
	m_WDL->CreateInsances(weak_from_this());
}

void CMap::Unload()
{
	m_WDL.reset();
	m_WDT.reset();
	m_SkyManager.reset();

	for (int i = 0; i < C_TilesCacheSize; i++)
	{
		if (m_ADTCache[i] != nullptr)
		{
			m_ADTCache[i].reset();
		}
	}

	for (int i = 0; i < C_RenderedTiles; i++)
	{
		for (int j = 0; j < C_RenderedTiles; j++)
		{
			m_Current[i][j].reset();
		}
	}
}

// --

void CMap::UpdateCamera(const Camera* camera)
{
	bool loading = false;
	int enteredTileX, enteredTileZ;
	int midTile = static_cast<uint32>(C_RenderedTiles / 2);
	if (m_Current[midTile][midTile] != nullptr || m_IsOnInvalidTile)
	{
        std::shared_ptr<CTransformComponent> transformComponent = m_Current[midTile][midTile]->GetComponent<CTransformComponent>();

		if (m_IsOnInvalidTile ||
			(camera->GetTranslation().x < transformComponent->GetTranslation().x) ||
			(camera->GetTranslation().x > (transformComponent->GetTranslation().x + C_TileSize)) ||
			(camera->GetTranslation().z < transformComponent->GetTranslation().z) ||
			(camera->GetTranslation().z > (transformComponent->GetTranslation().z + C_TileSize)))
		{

			enteredTileX = static_cast<int>(camera->GetTranslation().x / C_TileSize);
			enteredTileZ = static_cast<int>(camera->GetTranslation().z / C_TileSize);

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

	if (m_WDL)
		m_WDL->UpdateCamera(camera);
}

//--

void CMap::EnterMap(vec3 _cameraPosition)
{
	EnterMap(_cameraPosition.x / C_TileSize, _cameraPosition.z / C_TileSize);
}

void CMap::EnterMap(int32 x, int32 z)
{
	if (IsBadTileIndex(x, z) || !m_WDT->getTileFlags(x, z).Flag_HasADT)
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

std::shared_ptr<CMapTile> CMap::LoadTile(int32 x, int32 z)
{
	if (IsBadTileIndex(x, z))
	{
		return nullptr;
	}

	if (!m_WDT->getTileFlags(x, z).Flag_HasADT)
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
		m_ADTCache[maxidx].reset();
		firstnull = maxidx;
	}

	// Create new tile
	m_ADTCache[firstnull] = CreateSceneNode<CMapTile>();
    m_ADTCache[firstnull]->Initialize(x, z);
	Application::Get().GetLoader()->AddToLoadQueue(m_ADTCache[firstnull]);
	return m_ADTCache[firstnull];
}

void CMap::ClearCache()
{
	for (int i = 0; i < C_TilesCacheSize; i++)
	{
		if (m_ADTCache[i] != nullptr && !IsTileInCurrent(m_ADTCache[i]))
		{
			m_ADTCache[i].reset();
		}
	}
}

uint32 CMap::GetAreaID(Camera* camera)
{
	if (!m_WDT->MapHasTiles())
	{
		return UINT32_MAX;
	}

	int32 tileX = (int)(camera->GetTranslation().x / C_TileSize);
	int32 tileZ = (int)(camera->GetTranslation().z / C_TileSize);

	int32 chunkX = (int)(fmod(camera->GetTranslation().x, C_TileSize) / C_ChunkSize);
	int32 chunkZ = (int)(fmod(camera->GetTranslation().z, C_TileSize) / C_ChunkSize);

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

	std::shared_ptr<CMapTile> curTile = m_Current[indexX][indexY];
	if (curTile == nullptr)
	{
		return UINT32_MAX;
	}

	std::shared_ptr<CMapChunk> curChunk = curTile->getChunk(chunkX, chunkZ);
	if (curChunk == nullptr)
	{
		return UINT32_MAX;
	}

	return curChunk->GetAreaID();
}

bool CMap::getTileIsCurrent(int x, int z) const
{
    int midTile = static_cast<uint32>(C_RenderedTiles / 2);
    std::shared_ptr<CMapTile> currentTile = m_Current[midTile][midTile];
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

bool CMap::IsTileInCurrent(std::shared_ptr<CMapTile> _mapTile)
{
	for (int i = 0; i < C_RenderedTiles; i++)
		for (int j = 0; j < C_RenderedTiles; j++)
			if (m_Current[i][j] != nullptr)
				if (m_Current[i][j] == _mapTile)
					return true;
	return false;
}
