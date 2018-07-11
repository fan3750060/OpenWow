#include "stdafx.h"

// Include
#include "WMO.h"
#include "WMO_Group_Instance.h"

// General
#include "WMO_Group.h"

// Additional 
#include "WMO_Doodad_Instance.h"
#include "WMO_Liquid_Instance.h"
#include "WMO_Fixes.h"

WMO_Group::WMO_Group(const WMO* _parentWMO, const uint32 _groupIndex, string _groupName, IFile* _groupFile) :
	m_ParentWMO(_parentWMO),
	m_GroupName(_groupName),
	m_GroupIndex(_groupIndex),
	m_F(_groupFile),
	m_IsMOCVExists(false),
	m_Quality(GetSettingsGroup<CGroupQuality>())
{
	m_WMOLiqiud = nullptr;
}

WMO_Group::~WMO_Group()
{
	ERASE_VECTOR(m_WMOBatchIndexes);
	ERASE_VECTOR(m_CollisionNodes);
}

void WMO_Group::CreateInsances(CWMO_Group_Instance* _parent) const
{
	if (m_WMOLiqiud.getPtr() != nullptr)
	{
		vec3 realPos = m_LiquidHeader.pos.toXZmY();
		realPos.y = 0.0f; // why they do this???

		CWMO_Liquid_Instance* liquid = new CWMO_Liquid_Instance(_parent, m_WMOLiqiud, realPos, this);
		// add to scene node is automaticly
	}

	for (auto& index : m_DoodadsPlacementIndexes)
	{
		const SWMO_Doodad_PlacementInfo& placement = m_ParentWMO->m_DoodadsPlacementInfos[index];

		SharedPtr<M2> mdx = (M2*)GetManager<IM2Manager>()->Add(m_ParentWMO->m_DoodadsFilenames + placement.flags.nameIndex);
		CWMO_Doodad_Instance* instance = nullptr;
		if (mdx)
		{
			instance = new CWMO_Doodad_Instance(_parent, mdx, this, index, placement);
			// add to scene node is automaticly
		}
		_parent->addDoodadInstance(instance);
	}
}

uint32 WMO_Group::to_wmo_liquid(int x)
{
	DBC_LIQUIDTYPE_Type::List basic = (DBC_LIQUIDTYPE_Type::List)(x & 3);
	switch (basic)
	{
	case DBC_LIQUIDTYPE_Type::water:
		return (m_Header.flags.IS_NOT_WATER_BUT_OCEAN) ? 14 : 13;
	case DBC_LIQUIDTYPE_Type::ocean:
		return 14;
	case DBC_LIQUIDTYPE_Type::magma:
		return 19;
	case DBC_LIQUIDTYPE_Type::slime:
		return 20;
	}
}

void WMO_Group::Load()
{
	// Buffer
	uint16* dataFromMOVI = nullptr;
	SharedBufferPtr	IB_Default = nullptr;
	dataFromMOVT = nullptr;
	SharedBufferPtr	VB_Vertexes = nullptr;
	vector<SharedBufferPtr>	VB_TextureCoords;
	SharedBufferPtr	VB_Normals = nullptr;
	SharedBufferPtr	VB_Colors = nullptr;

	// CollisionTEMP
	uint32 collisionCount = 0;
	SWMO_Group_MOBNDef* collisions = nullptr;

	// Read file
	char fourcc[5];
	uint32 size = 0;
	while (!m_F->isEof())
	{
		memset(fourcc, 0, 4);
		size = 0;
		m_F->readBytes(fourcc, 4);
		m_F->readBytes(&size, 4);
		flipcc(fourcc);
		fourcc[4] = 0;
		if (size == 0)	continue;
		uint32_t nextpos = m_F->getPos() + size;

		if (strcmp(fourcc, "MVER") == 0)
		{
			uint32 version;
			m_F->readBytes(&version, 4);
			assert1(version == 17);
		}
		else if (strcmp(fourcc, "MOGP") == 0)
		{
			// The MOGP chunk size will be way more than the header variables!
			nextpos = m_F->getPos() + sizeof(SWMO_Group_HeaderDef);

			m_F->readBytes(&m_Header, sizeof(SWMO_Group_HeaderDef));

			assert1(m_Header.flags.HAS_3_MOTV == 0);

			// Bounds
			m_Bounds.set(m_Header.boundingBox.min, m_Header.boundingBox.max, true);
		}
		else if (strcmp(fourcc, "MOPY") == 0) // Material info for triangles
		{
			uint32 materialsInfoCount = size / sizeof(SWMO_Group_MaterialDef);
			SWMO_Group_MaterialDef* materialsInfo = (SWMO_Group_MaterialDef*)m_F->getDataFromCurrent();
			for (uint32 i = 0; i < materialsInfoCount; i++)
			{
				m_MaterialsInfo.push_back(materialsInfo[i]);
			}
		}
		else if (strcmp(fourcc, "MOVI") == 0) // Indices
		{
			assert1(IB_Default == nullptr);
			uint32 indicesCount = size / sizeof(uint16);
			uint16* indices = (uint16*)m_F->getDataFromCurrent();
			// Buffer
			IB_Default = _Render->r.createIndexBuffer(indicesCount * sizeof(uint16), indices, false);

			dataFromMOVI = indices;
		}
		else if (strcmp(fourcc, "MOVT") == 0) // Vertices chunk.
		{
			assert1(VB_Vertexes == nullptr);
			uint32 vertexesCount = size / sizeof(vec3);
			vec3* vertexes = (vec3*)m_F->getDataFromCurrent();
			// Convert
			for (uint32 i = 0; i < vertexesCount; i++)
			{
				vertexes[i] = vertexes[i].toXZmY();
			}
			// Buffer
			VB_Vertexes = _Render->r.createVertexBuffer(vertexesCount * sizeof(vec3), vertexes, false);
			VB_Colors = _Render->r.createVertexBuffer(vertexesCount * sizeof(vec4), nullptr, false);
			//m_Bounds.calculate(vertexes, vertexesCount);

			dataFromMOVT = vertexes;
		}
		else if (strcmp(fourcc, "MONR") == 0) // Normals
		{
			assert1(VB_Normals == nullptr);
			uint32 normalsCount = size / sizeof(vec3);
			vec3* normals = (vec3*)m_F->getDataFromCurrent();
			// Convert
			for (uint32 i = 0; i < normalsCount; i++)
			{
				normals[i] = normals[i].toXZmY();
			}
			// Buffer
			VB_Normals = _Render->r.createVertexBuffer(normalsCount * sizeof(vec3), normals, false);
		}
		else if (strcmp(fourcc, "MOTV") == 0) // R_Texture coordinates
		{
			uint32 textureCoordsCount = size / sizeof(vec2);
			vec2* textureCoords = (vec2*)m_F->getDataFromCurrent();
			VB_TextureCoords.push_back(_Render->r.createVertexBuffer(textureCoordsCount * sizeof(vec2), textureCoords, false));
		}
		else if (strcmp(fourcc, "MOBA") == 0) // WMO_Group_Batch
		{
			uint32 batchesCount = size / sizeof(SWMO_Group_BatchDef);
			SWMO_Group_BatchDef* batches = (SWMO_Group_BatchDef*)m_F->getDataFromCurrent();
			moba = new SWMO_Group_BatchDef[batchesCount];
			memcpy(moba, batches, sizeof(SWMO_Group_BatchDef) * batchesCount);

			for (uint32 i = 0; i < size / sizeof(SWMO_Group_BatchDef); i++)
			{
				SWMO_Group_BatchDef batchDef;
				m_F->readBytes(&batchDef, sizeof(SWMO_Group_BatchDef));

				WMO_Group_Part_Batch* batch = new WMO_Group_Part_Batch(m_ParentWMO, this, batchDef);
				m_WMOBatchIndexes.push_back(batch);
			}

			std::sort(m_WMOBatchIndexes.begin(), m_WMOBatchIndexes.end(), WMO_Group_Part_BatchCompare());
		}
		else if (strcmp(fourcc, "MOLR") == 0) // Light references
		{
			assert1(m_Header.flags.HAS_LIGHTS);
			uint32 lightsIndexesCount = size / sizeof(uint16);
			uint16* lightsIndexes = (uint16*)m_F->getDataFromCurrent();
			for (uint32 i = 0; i < lightsIndexesCount; i++)
			{
				m_WMOLightsIndexes.push_back(lightsIndexes[i]);
			}
		}
		else if (strcmp(fourcc, "MODR") == 0) // Doodad references
		{
			assert1(m_Header.flags.HAS_DOODADS);
			uint32 doodadsIndexesCount = size / sizeof(uint16);
			uint16* doodadsIndexes = (uint16*)m_F->getDataFromCurrent();
			for (uint32 i = 0; i < doodadsIndexesCount; i++)
			{
				uint16 index = doodadsIndexes[i];
				m_DoodadsPlacementIndexes.push_back(index);
			}
		}
		else if (strcmp(fourcc, "MOBN") == 0)
		{
			assert1(m_Header.flags.HAS_COLLISION);

			collisionCount = size / sizeof(SWMO_Group_MOBNDef);
			collisions = (SWMO_Group_MOBNDef*)m_F->getDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOBR") == 0)
		{
			uint32 indexesCnt = size / sizeof(uint16);
			uint16* indices = (uint16*)m_F->getDataFromCurrent();

			/*collisionIndexes.reserve(indexesCnt * 3);
			for (uint32 i = 0; i < indexesCnt; i++)
			{
				collisionIndexes[i * 3 + 0] = dataFromMOVI[3 * indices[i] + 0];
				collisionIndexes[i * 3 + 1] = dataFromMOVI[3 * indices[i] + 1];
				collisionIndexes[i * 3 + 2] = dataFromMOVI[3 * indices[i] + 2];
			}*/
		}
		else if (strcmp(fourcc, "MOCV") == 0) // Vertex colors
		{
			assert1(m_Header.flags.HAS_VERTEX_COLORS);
			uint32 vertexColorsCount = size / sizeof(CBgra);
			CBgra* vertexColors = (CBgra*)m_F->getDataFromCurrent();
			mocv = new C4Vec[vertexColorsCount];
			memcpy(mocv, vertexColors, sizeof(C4Vec) * vertexColorsCount);
			mocv_count = vertexColorsCount;

			FixColorVertexAlpha(this);

			// Convert
			vector<vec4> vertexColorsConverted;
			for (uint32 i = 0; i < vertexColorsCount; i++)
			{
				vertexColorsConverted.push_back(vec4
				(
					static_cast<float>(mocv[i].z) / 255.0f,
					static_cast<float>(mocv[i].y) / 255.0f,
					static_cast<float>(mocv[i].x) / 255.0f,
					static_cast<float>(mocv[i].w) / 255.0f
				));
			}

			// Buffer
			VB_Colors = _Render->r.createVertexBuffer(vertexColorsConverted.size() * sizeof(vec4), vertexColorsConverted.data(), false);
			m_IsMOCVExists = vertexColorsCount > 0;
		}
		else if (strcmp(fourcc, "MLIQ") == 0) // Liquid
		{
			m_F->readBytes(&m_LiquidHeader, sizeof(SWMO_Group_MLIQDef));

			Log::Green("WMO[%s]: LiquidType CHUNK = HEADER[%d] [%d]", m_ParentWMO->m_FileName.c_str(), m_Header.liquidType, m_ParentWMO->m_Header.flags.use_liquid_type_dbc_id);

			uint32 liquid_type;
			if (m_ParentWMO->m_Header.flags.use_liquid_type_dbc_id)
			{
				if (m_Header.liquidType < 21)
				{
					liquid_type = to_wmo_liquid(m_Header.liquidType - 1);
				}
				else
				{
					liquid_type = m_Header.liquidType;
				}
			}
			else
			{
				if (m_Header.liquidType < 20)
				{
					liquid_type = to_wmo_liquid(m_Header.liquidType);
				}
				else
				{
					liquid_type = m_Header.liquidType + 1;
				}
			}

			m_WMOLiqiud = new CWMO_Liquid(m_LiquidHeader.A, m_LiquidHeader.B);
			m_WMOLiqiud->CreateFromWMO(m_F, m_ParentWMO->m_Materials[m_LiquidHeader.materialID], DBC_LiquidType[liquid_type], m_Header.flags.IS_INDOOR);

		}
		else
		{
			Log::Fatal("WMO_Group[]: No implement group chunk %s [%d].", fourcc, size);
		}
		m_F->seek(nextpos);
	}

	for (uint32 i = 0; i < collisionCount; i++)
	{
		CWMO_Group_Part_BSP_Node* collisionNode = new CWMO_Group_Part_BSP_Node(this, collisions[i]);
		m_CollisionNodes.push_back(collisionNode);
	}

	delete m_F;
	m_F = nullptr;

	// Create geom
	{
		__geom = _Render->r.beginCreatingGeometry(PRIM_TRILIST, _Render->getRenderStorage()->__layout_GxVBF_PNCT2);
		__geom->setGeomVertexParams(VB_Vertexes, R_DataType::T_FLOAT, 0, 0);
		__geom->setGeomVertexParams(VB_Normals, R_DataType::T_FLOAT, 0, 0);
		__geom->setGeomVertexParams(VB_Colors, R_DataType::T_FLOAT, 0, 0);
		__geom->setGeomVertexParams(VB_TextureCoords[0], R_DataType::T_FLOAT, 0, 0);
		__geom->setGeomVertexParams((VB_TextureCoords.size() == 2) ? VB_TextureCoords[1] : VB_TextureCoords[0], R_DataType::T_FLOAT, 0, 0);
		__geom->setGeomIndexParams(IB_Default, R_IndexFormat::IDXFMT_16);
		__geom->finishCreatingGeometry();
	}

	for (auto& batch : m_WMOBatchIndexes)
	{
		batch->setGeom(__geom);
	}

	// Create collision geom
	{

	}
}

void WMO_Group::initLighting()
{
	/*if (m_Header.flags.FLAG_IS_INDOOR && m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		vec3 dirmin(1, 1, 1);
		float lenmin;
		int lmin;
		for (uint32 i = 0; i < m_DoodadsIndexesCount; i++)
		{
			lenmin = 999999.0f * 999999.0f;
			lmin = 0;
			DoodadInstance* mi = m_ParentWMO->m_M2Instances[m_PlacementInfoIndexes[i]];
			for (uint32 j = 0; j < m_ParentWMO->m_Header.nLights; j++)
			{
				WMO_Part_Light* l = m_ParentWMO->m_Lights[j];
				vec3 dir = l->lightDef.pos - mi->placementInfo->position;
				float ll = dir.length2();
				if (ll < lenmin)
				{
					lenmin = ll;
					dirmin = dir;
					lmin = j;
				}
			}
			mi->light = lmin;
			mi->ldir = dirmin;
		}
		m_EnableOutdoorLights = false;
	}
	else
	{
		m_EnableOutdoorLights = true;
	}*/
}

//



void WMO_Group::Render(cmat4 _world) const
{
	PERF_START(PERF_MAP_MODELS_WMOs_GEOMETRY);
	{
		CWMO_GeomertyPass* pass = _Render->getTechniquesMgr()->WMO_Pass;
		pass->Bind();
		{
			pass->setWorld(_world);

			// Vertex colors
			pass->SetHasMOCV(m_IsMOCVExists && m_Quality.WMO_MOCV);

			// Ambient color
			pass->SetUseAmbColor(m_Quality.WMO_AmbColor);
			//pass->SetAmbColor(m_ParentWMO->m_Header.getAmbColor());

			for (auto it : m_WMOBatchIndexes)
			{
				it->Render();
			}
		}
		pass->Unbind();
	}
	PERF_STOP(PERF_MAP_MODELS_WMOs_GEOMETRY);

	//RenderCollision(_world);
}

void WMO_Group::RenderCollision(cmat4 _world) const
{
	for (auto& node : m_CollisionNodes)
	{
		node->Render(_world);
	}
}
