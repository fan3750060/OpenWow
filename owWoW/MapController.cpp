#include "stdafx.h"

// General
#include "MapController.h"

// Additional
#include "EnvironmentManager.h"
#include "WorldController.h"
#include "Map_Shared.h"

MapController::MapController() : 
    m_GlobalWMO(nullptr),
    m_GlobalWMOPlacementInfo(nullptr)
{
    // Load default variables
    m_IsTileBased = false;
    memset(m_LowResilutionTiles, 0, sizeof(m_LowResilutionTiles));
    minimap = 0;
    memset(m_MapTilesCache, 0, sizeof(m_MapTilesCache));
    m_CurrentTileX = m_CurrentTileZ = -1;
    memset(m_Current, 0, sizeof(m_Current));
    m_IsOnInvalidTile = false;

    Map_Shared::CreateMapArrays();

    ADDCONSOLECOMMAND_CLASS("map_clear", MapController, ClearCache);
}

MapController::~MapController()
{
}

//

void MapController::InitGlobalsWMOs()
{
    // Load global WMO

    Log::Green("Map_GlobalWMOs[]: Global WMO exists [%s].", m_GlobalWMOPlacementInfo != nullptr ? "true" : "false");
    if (m_GlobalWMOPlacementInfo != nullptr)
    {
        WMO* wmo = _World->WMOM()->Add(m_GlobalWMOName);
        m_GlobalWMO = new WMOInstance(wmo, *m_GlobalWMOPlacementInfo);
    }

    // Load low-resolution WMOs

    Log::Green("Map_GlobalWMOs[]: Low WMOs count [%d].", m_LowResolutionWMOsPlacementInfo.size());
    for (auto it : m_LowResolutionWMOsPlacementInfo)
    {
        const string name = m_LowResolutionWMOsNames[it->nameIndex];

        WMO* wmo = _World->WMOM()->Add(name);
        WMOInstance* inst = new WMOInstance(wmo, *it);
        m_LowResolutionWMOs.push_back(inst);
    }
}

//

void MapController::Load_WDT(DBC_MapRecord* _map)
{
    m_DBC_Map = _map;

    Log::Print("Map[%s]: Id [%d]. Preloading...", m_DBC_Map->Get_Directory(), m_DBC_Map->Get_ID());

    m_MapFolder = "World\\Maps\\" + string(m_DBC_Map->Get_Directory()) + "\\";

    // Init main section
    File f(m_MapFolder + m_DBC_Map->Get_Directory() + ".wdt");
    if (!f.Open())
    {
        Log::Info("Map[%s]: WDT: Error opening.", m_DBC_Map->Get_Directory());
        return;
    }

    // Load sky
	_World->EnvM()->InitSkies(m_DBC_Map);

    char fourcc[5];
    uint32 size;

    while (!f.IsEof())
    {
        memset(fourcc, 0, 4);
        size = 0;
        f.ReadBytes(fourcc, 4);
        f.ReadBytes(&size, 4);
        flipcc(fourcc);
        fourcc[4] = 0;
        if (size == 0) continue;
        uint32_t nextpos = f.GetPos() + size;

        if (strcmp(fourcc, "MVER") == 0)
        {
            uint32 version;
            f.ReadBytes(&version, 4);
            assert1(version == 18);
        }
        else if (strcmp(fourcc, "MPHD") == 0)
        {
            // unit32 flags
            // unit32 something
            // unit32 unk[6]

            f.ReadBytes(&m_Flag, 4);
        }
        else if (strcmp(fourcc, "MAIN") == 0) // Map tile table. Contains 64x64 = 4096 records of 8 bytes each.
        {
            for (int i = 0; i < 64; i++)
            {
                for (int j = 0; j < 64; j++)
                {
                    // Flag
                    f.ReadBytes(&m_TileFlag[j][i], 4);

                    if (m_TileFlag[j][i].Flag_HasTerrain)
                    {
                        m_IsTileBased = true;
                    }

                    // UNUSED: Assync ID
                    uint32 asyncId;
                    f.ReadBytes(&asyncId, 4);
                }
            }
        }
        else if (strcmp(fourcc, "MWMO") == 0) // Global WMO filename
        {
            if (size > 0)
            {
                char* buf = new char[size];
                f.ReadBytes(buf, size);

                m_GlobalWMOName = string(buf);

                delete[] buf;
            }
        }
        else if (strcmp(fourcc, "MODF") == 0)
        {
            assert1((size / WMOPlacementInfo::__size) == 1);
            m_GlobalWMOPlacementInfo = new WMOPlacementInfo;
            f.ReadBytes(m_GlobalWMOPlacementInfo, WMOPlacementInfo::__size);
        }
        else
        {
            Log::Info("Map[%s]: WDT: Chunks [%s], Size [%d] not implemented.", m_DBC_Map->Get_Directory(), fourcc, size);
        }

        f.Seek(nextpos);
    }

    Load_WDL();
}

void MapController::Load_WDL()
{
    File f(m_MapFolder + m_DBC_Map->Get_Directory() + ".wdl");
    if (!f.Open())
    {
        Log::Info("World[%s]: WDL: Error opening.", m_DBC_Map->Get_Directory());
        return;
    }

    // Offsets to MARE
    uint32 MARE_Offsets[C_TilesInMap][C_TilesInMap];
    memset(MARE_Offsets, 0x00, C_TilesInMap * C_TilesInMap * sizeof(uint32));

    char fourcc[5];
    uint32 size;

    while (!f.IsEof())
    {
        memset(fourcc, 0, 4);
        size = 0;
        f.ReadBytes(fourcc, 4);
        f.ReadBytes(&size, 4);
        flipcc(fourcc);
        fourcc[4] = 0;
        if (size == 0) continue;
        uint32_t nextpos = f.GetPos() + size;

        if (strcmp(fourcc, "MVER") == 0)
        {
            uint32 version;
            f.ReadBytes(&version, 4);
            assert3(version == 18, "Version mismatch != 18", std::to_string(version).c_str());
        }
        else if (strncmp(fourcc, "MWMO", 4) == 0) // Filenames for WMO that appear in the low resolution map. Zero terminated strings.
        {
            WOWCHUNK_READ_STRINGS_BEGIN

                m_LowResolutionWMOsNames.push_back(_string);
            WOWCHUNK_READ_STRINGS_END;
        }
        else if (strncmp(fourcc, "MWID", 4) == 0) // List of indexes into the MWMO chunk.
        {
        }
        else if (strncmp(fourcc, "MODF", 4) == 0) // Placement information for the WMO. Appears to be the same 64 byte structure used in the WDT and ADT MODF chunks.
        {
            for (uint32 i = 0; i < size / WMOPlacementInfo::__size; i++)
            {
                WMOPlacementInfo* placement = new WMOPlacementInfo;
                f.ReadBytes(placement, WMOPlacementInfo::__size);
                m_LowResolutionWMOsPlacementInfo.push_back(placement);
            }
        }
        else if (strncmp(fourcc, "MAOF", 4) == 0) // Contains 64*64 = 4096 unsigned 32-bit integers, these are absolute offsets in the file to each map tile's MapAreaLow-array-entry. For unused tiles the value is 0.
        {
            f.ReadBytes(MARE_Offsets, C_TilesInMap * C_TilesInMap * sizeof(uint32));
        }
        else if (strncmp(fourcc, "MARE", 4) == 0) // Heightmap for one map tile.
        {
            // Contains 17 * 17 + 16 * 16 = 545 signed 16-bit integers. So a 17 by 17 grid of height values is given, with additional height values in between grid points. Here, the "outer" 17x17 points are listed (in the usual row major order), followed by 16x16 "inner" points. The height values are on the same scale as those used in the regular height maps.
        }
        else if (strncmp(fourcc, "MAHO", 4) == 0)
        {
            // After each MARE chunk there follows a MAHO (MapAreaHOles) chunk. It may be left out if the data is supposed to be 0 all the time. Its an array of 16 shorts. Each short is a bitmask. If the bit is not set, there is a hole at this position.
        }
        else
        {
            Log::Info("Map[%s]: WDL: Chunks [%s], Size [%d] not implemented.", m_DBC_Map->Get_Directory(), fourcc, size);
        }
        f.Seek(nextpos);
    }

    // Minimap
    uint32* texbuf = new uint32[512 * 512];
    memset(texbuf, 0, 512 * 512 * 4);

    // Heightmap
    vec3 lowres[17][17];
    vec3 lowsub[16][16];

    for (int j = 0; j < 64; j++)
    {
        for (int i = 0; i < 64; i++)
        {
            if (MARE_Offsets[j][i])
            {
                // Read data             
                
                f.Seek(MARE_Offsets[j][i] + 4 + 4);

                short tilebuf[17 * 17];
                f.ReadBytes(tilebuf, 17 * 17 * 2);

                short tilebuf2[16 * 16];
                f.ReadBytes(tilebuf2, 16 * 16 * 2);

                // make minimap
                // for a 512x512 minimap texture, and 64x64 tiles, one tile is 8x8 pixels
                for (int z = 0; z < 8; z++)
                {
                    for (int x = 0; x < 8; x++)
                    {
                        short hval = tilebuf[(z * 2) * 17 + x * 2]; // for now
                        uint8 r, g, b;
                        if (hval < 0)
                        {
                            // water = blue
                            if (hval < -511) hval = -511;
                            hval /= -2;
                            r = g = 0;
                            b = 255 - hval;
                        }
                        else
                        {
                            // above water = should apply a palette :(
                            /*
                            float fh = hval / 1600.0f;
                            if (fh > 1.0f) fh = 1.0f;
                            uint8 c = (uint8) (fh * 255.0f);
                            r = g = b = c;
                            */

                            // green: 20,149,7		0-600
                            // brown: 137, 84, 21	600-1200
                            // gray: 96, 96, 96		1200-1600
                            // white: 255, 255, 255
                            uint8 r1, r2, g1, g2, b1, b2;
                            float t;

                            if (hval < 600)
                            {
                                r1 = 20;
                                r2 = 137;
                                g1 = 149;
                                g2 = 84;
                                b1 = 7;
                                b2 = 21;
                                t = hval / 600.0f;
                            }
                            else if (hval < 1200)
                            {
                                r2 = 96;
                                r1 = 137;
                                g2 = 96;
                                g1 = 84;
                                b2 = 96;
                                b1 = 21;
                                t = (hval - 600) / 600.0f;
                            }
                            else /*if (hval < 1600)*/
                            {
                                r1 = 96;
                                r2 = 255;
                                g1 = 96;
                                g2 = 255;
                                b1 = 96;
                                b2 = 255;
                                if (hval >= 1600) hval = 1599;
                                t = (hval - 1200) / 600.0f;
                            }

                            // TODO: add a regular palette here

                            r = (uint8)(r2*t + r1*(1.0f - t));
                            g = (uint8)(g2*t + g1*(1.0f - t));
                            b = (uint8)(b2*t + b1*(1.0f - t));
                        }

                        texbuf[(j * 8 + z) * 512 + i * 8 + x] = (r) | (g << 8) | (b << 16) | (255 << 24);
                    }
                }

                for (int y = 0; y < 17; y++)
                {
                    for (int x = 0; x < 17; x++)
                    {
                        lowres[y][x] = vec3(C_TileSize*(i + x / 16.0f), tilebuf[y * 17 + x], C_TileSize*(j + y / 16.0f));
                    }
                }

                for (int y = 0; y < 16; y++)
                {
                    for (int x = 0; x < 16; x++)
                    {
                        lowsub[y][x] = vec3(C_TileSize*(i + (x + 0.5f) / 16.0f), tilebuf2[y * 16 + x], C_TileSize*(j + (y + 0.5f) / 16.0f));
                    }
                }


                vector<vec3> vecrtices;

                for (int y = 0; y < 16; y++)
                {
                    for (int x = 0; x < 16; x++)
                    {
                        vecrtices.push_back(lowres[y][x]);
                        vecrtices.push_back(lowsub[y][x]);
                        vecrtices.push_back(lowres[y][x + 1]);

                        vecrtices.push_back(lowres[y][x + 1]);
                        vecrtices.push_back(lowsub[y][x]);
                        vecrtices.push_back(lowres[y + 1][x + 1]);

                        vecrtices.push_back(lowres[y + 1][x + 1]);
                        vecrtices.push_back(lowsub[y][x]);
                        vecrtices.push_back(lowres[y + 1][x]);

                        vecrtices.push_back(lowres[y + 1][x]);
                        vecrtices.push_back(lowsub[y][x]);
                        vecrtices.push_back(lowres[y][x]);
                    }
                }

                // Vertex buffer
                R_Buffer* __vb = _Render->r.createVertexBuffer(vecrtices.size() * sizeof(vec3), vecrtices.data());

                //

                m_LowResilutionTiles[j][i] = _Render->r.beginCreatingGeometry(_Render->Storage()->__layout_GxVBF_P);

                // Vertex params
				m_LowResilutionTiles[j][i]->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0, 0);

                // Index bufer
                //uint32 __ib = _Render->r.createIndexBuffer(striplen, strip);
                //_Render->r.setGeomIndexParams(lowrestiles[j][i], __ib, R_IndexFormat::IDXFMT_16);

                // Finish
				m_LowResilutionTiles[j][i]->finishCreatingGeometry();
            }
        }
    }

    // Finish minimap
    minimap = _Render->r.createTexture(R_TextureTypes::Tex2D, 512, 512, 1, R_TextureFormats::RGBA8, false, false, false, false);
	minimap->uploadTextureData(0, 0, texbuf);
    delete[] texbuf;


    // Init global WMOs
    InitGlobalsWMOs();
}

void MapController::Unload()
{
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            if (m_LowResilutionTiles[i][j] != 0)
            {
				m_LowResilutionTiles[i][j]->destroyGeometry(true);
            }
        }
    }

    for (int i = 0; i < C_TilesCacheSize; i++)
    {
        if (m_MapTilesCache[i] != nullptr)
        {
            delete m_MapTilesCache[i];
        }
    }

    if (minimap)
    {
        //glDeleteTextures(1, &minimap);
    }
}

//

void MapController::Tick()
{
    bool loading = false;
    int enteredTileX, enteredTileZ;
    int midTile = static_cast<uint32>(C_RenderedTiles / 2);
    if (m_Current[midTile][midTile] != nullptr || m_IsOnInvalidTile)
    {
        if (m_IsOnInvalidTile ||
            (_Camera->Position.x < m_Current[midTile][midTile]->m_GamePositionX) ||
            (_Camera->Position.x > (m_Current[midTile][midTile]->m_GamePositionX + C_TileSize)) ||
            (_Camera->Position.z < m_Current[midTile][midTile]->m_GamePositionZ) ||
            (_Camera->Position.z > (m_Current[midTile][midTile]->m_GamePositionZ + C_TileSize)))
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

void MapController::RenderSky()
{
    for (int i = 0; i < C_RenderedTiles; i++)
    {
        for (int j = 0; j < C_RenderedTiles; j++)
        {
            if (m_Current[i][j] != nullptr)
            {
                m_Current[i][j]->drawSky();
            }

            if (_World->EnvM()->m_HasSky)
            {
                break;
            }
        }
    }
}

void MapController::RenderLowResTiles()
{
    for (int i = 0; i < C_TilesInMap; i++)
    {
        for (int j = 0; j < C_TilesInMap; j++)
        {
            if (m_LowResilutionTiles[i][j])
            {
                _Render->r.setGeometry(m_LowResilutionTiles[i][j]);

                _Render->r.draw(PRIM_TRILIST, 0, 16 * 16 * 12);
                PERF_INC(PERF_MAP_LOWRESOLUTION);
            }
        }
    }

    /*const int lrr = 5;
    for (int i = m_CurrentTileX - lrr; i <= m_CurrentTileX + lrr; i++)
    {
        for (int j = m_CurrentTileZ - lrr; j <= m_CurrentTileZ + lrr; j++)
        {
            // OOB check
            if (IsBadTileIndex(i, j))
            {
                continue;
            }

            // Don't draw current tile
            if (i == m_CurrentTileX && j == m_CurrentTileZ)
            {
                continue;
            }

            // TODO: some annoying visual artifacts when the verylowres terrain overlaps
            // maptiles that are close (1-off) - figure out how to fix.
            // still less annoying than hoels in the horizon when only 2-off verylowres tiles are drawn
            if (lowrestiles[i][j])
            {
                glBindBuffer(GL_ARRAY_BUFFER, lowrestiles[i][j]);

                // Vertex
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

                //glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_SHORT, strip);
                glDrawBuffer(GL_TRIANGLES);
                PERF_INC(PERF_MAP_LOWRES_TILES);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
        }
    }*/
}

void MapController::RenderTiles()
{
    // Draw cache
    for (int i = 0; i < C_TilesCacheSize; i++)
    {
        if (m_MapTilesCache[i] != nullptr)
        {
            m_MapTilesCache[i]->draw();
        }
    }

    // Draw current
    /*for (int i = 0; i < C_RenderedTiles; i++)
        for (int j = 0; j < C_RenderedTiles; j++)
            if (current[i][j] != nullptr)
                current[i][j]->draw();*/
}

void MapController::RenderObjects()
{
    for (int i = 0; i < C_RenderedTiles; i++)
        for (int j = 0; j < C_RenderedTiles; j++)
            if (m_Current[i][j] != nullptr)
                m_Current[i][j]->drawObjects();
}

void MapController::RenderModels()
{
    for (int i = 0; i < C_RenderedTiles; i++)
        for (int j = 0; j < C_RenderedTiles; j++)
            if (m_Current[i][j] != nullptr)
                m_Current[i][j]->drawModels();
}

void MapController::RenderWater()
{
    for (int i = 0; i < C_RenderedTiles; i++)
        for (int j = 0; j < C_RenderedTiles; j++)
            if (m_Current[i][j] != nullptr)
                m_Current[i][j]->drawWater();
}

void MapController::Render_DEBUG()
{
    // Draw cache
    for (int i = 0; i < C_TilesCacheSize; i++)
    {
        if (m_MapTilesCache[i] != nullptr)
        {
            m_MapTilesCache[i]->Render_DEBUG();
        }
    }
}

//

void MapController::EnterMap(int32 x, int32 z)
{
    if (IsBadTileIndex(x, z) || !m_TileFlag[x][z].Flag_HasTerrain)
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

MapTile* MapController::LoadTile(int32 x, int32 z)
{
    if (IsBadTileIndex(x, z))
    {
        return nullptr;
    }

    if (!m_TileFlag[x][z].Flag_HasTerrain)
    {
        return nullptr;
    }

    // Try get tile from cache
    int firstnull = C_TilesCacheSize;
    for (int i = 0; i < C_TilesCacheSize; i++)
    {
        if ((m_MapTilesCache[i] != nullptr) && (m_MapTilesCache[i]->m_IndexX == x) && (m_MapTilesCache[i]->m_IndexZ == z))
        {
            return m_MapTilesCache[i];
        }

        if ((m_MapTilesCache[i] == nullptr) && (i < firstnull))
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
            if (m_MapTilesCache[i] == nullptr)
            {
                continue;
            }

            score = abs(m_MapTilesCache[i]->m_IndexX - m_CurrentTileX) + abs(m_MapTilesCache[i]->m_IndexZ - m_CurrentTileZ);

            if (score > maxscore)
            {
                maxscore = score;
                maxidx = i;
            }
        }

        // maxidx is the winner (loser)
        delete m_MapTilesCache[maxidx];
        m_MapTilesCache[maxidx] = nullptr;
        firstnull = maxidx;
    }

    // Create new tile
    m_MapTilesCache[firstnull] = new MapTile(x, z);
    if (!m_MapTilesCache[firstnull]->Load(m_DBC_Map->Get_Directory()))
    {
        delete m_MapTilesCache[firstnull];
        Log::Info("Map[%d]: Error loading tile [%d, %d].", m_MapFolder.c_str(), x, z);
        return nullptr;
    }

    return m_MapTilesCache[firstnull];
}

void MapController::ClearCache()
{
    for (int i = 0; i < C_TilesCacheSize; i++)
    {
        if (m_MapTilesCache[i] != nullptr && !IsTileInCurrent(m_MapTilesCache[i]))
        {
            delete m_MapTilesCache[i];
            m_MapTilesCache[i] = nullptr;
        }
    }
}

uint32 MapController::getAreaID()
{
    MapTile* curTile;

    int tileX, tileZ, chunkX, chunkZ;
    tileX = (int)(_Camera->Position.x / C_TileSize);
    tileZ = (int)(_Camera->Position.z / C_TileSize);

    chunkX = (int)(fmod(_Camera->Position.x, C_TileSize) / C_ChunkSize);
    chunkZ = (int)(fmod(_Camera->Position.z, C_TileSize) / C_ChunkSize);

    if (
        (tileX < m_CurrentTileX - static_cast<int>(C_RenderedTiles / 2)) || (tileX > m_CurrentTileX + static_cast<int>(C_RenderedTiles / 2)) ||
        (tileZ < m_CurrentTileZ - static_cast<int>(C_RenderedTiles / 2)) || (tileZ > m_CurrentTileZ + static_cast<int>(C_RenderedTiles / 2))
        )
        return 0;

    curTile = m_Current[tileZ - m_CurrentTileZ + static_cast<int>(C_RenderedTiles / 2)][tileX - m_CurrentTileX + static_cast<int>(C_RenderedTiles / 2)];
    if (curTile == 0)
        return 0;

    MapChunk* curChunk = curTile->getChunk(chunkX, chunkZ);

    if (curChunk == nullptr)
        return 0;

    return curChunk->areaID;
}

bool MapController::IsTileInCurrent(MapTile* _mapTile)
{
    for (int i = 0; i < C_RenderedTiles; i++)
        for (int j = 0; j < C_RenderedTiles; j++)
            if (m_Current[i][j] != nullptr)
                if (m_Current[i][j] == _mapTile)
                    return true;
    return false;
}
