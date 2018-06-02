#include "stdafx.h"

// Includes
#include "Wmo.h"

// General
#include "Wmo_Group.h"

// Additional
#include "Wmo_Fog.h"
#include "Wmo_Light.h"
#include "Wmo_Material.h"
#include "WorldController.h"

struct WMOGroupInfoDef
{
	WMOGroupFlags flags;
	CAaBox bounding_box;
	int32 nameoffset;                                   // name in MOGN chunk (-1 for no name)
};

struct WMO_Group_MLIQ
{
	uint32 X;
	uint32 Y;

	uint32 A;
	uint32 B;

	vec3 pos;

	uint16 type;
};

WMOGroup::WMOGroup(const WMO* _parentWMO, const uint32 _groupIndex, IFile* f) : 
	m_ParentWMO(_parentWMO), 
	m_GroupIndex(_groupIndex)
{
	m_MaterialsInfoCount = 0;
	m_MaterialsInfo = nullptr;

	m_Indices = nullptr;

	m_VertexesCount = 0;
	m_Vertexes = nullptr;

	m_Normals = nullptr;
	m_TextureCoords = nullptr;


	m_WMOBatchIndexesCount = 0;
	m_WMOBatchIndexes = nullptr;


	m_WMOLightsIndexesCount = 0;
	m_WMOLightsIndexes = nullptr;


	m_DoodadsIndexesCount = 0;
	m_DoodadsIndexes = nullptr;

	m_VertexColors = nullptr;

	m_LiquidInstance = nullptr;

	// Read hgroup info for bounding box and name
	WMOGroupInfoDef groupInfo;
	f->ReadBytes(&groupInfo, sizeof(WMOGroupInfoDef));

	if (groupInfo.nameoffset > 0)
	{
		m_GroupName = string(m_ParentWMO->m_GroupsNames + groupInfo.nameoffset);
	}
	else
	{
		m_GroupName = "(no name)";
	}

}

WMOGroup::~WMOGroup()
{
	delete[] m_WMOBatchIndexes;
	delete[] m_DoodadsIndexes;
}

void WMOGroup::Load()
{
	char temp[256];
	strcpy_s(temp, m_ParentWMO->m_FileName.c_str());
	temp[m_ParentWMO->m_FileName.length() - 4] = 0;

	char fname[256];
	sprintf_s(fname, "%s_%03d.wmo", temp, m_GroupIndex);

	UniquePtr<IFile> f = GetManager<IFilesManager>()->Open(fname);
	assert1(f != nullptr);

    //

	char fourcc[5];
	uint32 size = 0;
	while (!f->IsEof())
	{
		memset(fourcc, 0, 4);
		size = 0;
		f->ReadBytes(fourcc, 4);
		f->ReadBytes(&size, 4);
		flipcc(fourcc);
		fourcc[4] = 0;
		if (size == 0)	continue;
		uint32_t nextpos = f->GetPos() + size;

		if (strcmp(fourcc, "MVER") == 0)
		{
			uint32 version;
			f->ReadBytes(&version, 4);
			assert1(version == 17);
		}
		else if (strcmp(fourcc, "MOGP") == 0)
		{
			nextpos = f->GetPos() + sizeof(WMOGroupHeader); // The MOGP chunk size will be way more than the header variables!

			f->ReadBytes(&m_Header, sizeof(WMOGroupHeader));

			assert1(m_Header.flags.FLAG_HAS_2_MOCV == false);
			assert1(m_Header.flags.FLAG_HAS_2_MOTV == false);
			assert1(m_Header.flags.FLAG_HAS_3_MOTV == false);

			// Bounds
			m_Bounds.set(m_Header.boundingBox.min, m_Header.boundingBox.max, true);
		}
		else if (strcmp(fourcc, "MOPY") == 0) // Material info for triangles
		{
			m_MaterialsInfoCount = size / sizeof(WMOGroup_MaterialInfo);
			m_MaterialsInfo = (WMOGroup_MaterialInfo*)f->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOVI") == 0) // Vertex indices for triangles
		{
			m_IndicesCount = size / sizeof(uint16);
			m_Indices = (uint16*)f->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOVT") == 0) // Vertices chunk.
		{
			m_VertexesCount = size / sizeof(vec3);
			m_Vertexes = (vec3*)f->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MONR") == 0) // Normals
		{
			m_Normals = (vec3*)f->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOTV") == 0) // R_Texture coordinates
		{
			m_TextureCoords = (vec2*)f->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOBA") == 0) // Render m_WMOBatchIndexes.
		{
			m_WMOBatchIndexesCount = size / WMOBatch::__size;
			m_WMOBatchIndexes = new WMOBatch[m_WMOBatchIndexesCount];
			for (uint32 i = 0; i < m_WMOBatchIndexesCount; i++)
			{
				// Read
				f->ReadBytes(&m_WMOBatchIndexes[i], WMOBatch::__size);

				// Set material
				WMOBatch* _batch = &m_WMOBatchIndexes[i];
				WMOMaterial* _wmoMat = m_ParentWMO->m_Materials[_batch->material_id];

				_batch->__material.SetDiffuseTexture(_wmoMat->texture);

				_batch->__material.SetBlendState(_wmoMat->GetBlendMode());
				_batch->__material.SetRenderState(_wmoMat->IsTwoSided());
			}
		}
		else if (strcmp(fourcc, "MOLR") == 0) // Light references
		{
			assert1(m_Header.flags.FLAG_HAS_LIGHTS);
			m_WMOLightsIndexesCount = size / sizeof(uint16);
			m_WMOLightsIndexes = (uint16*)f->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MODR") == 0) // Doodad references
		{
			assert1(m_Header.flags.FLAG_HAS_DOODADS);
			m_DoodadsIndexesCount = size / sizeof(uint16);
			m_DoodadsIndexes = new uint16[size / sizeof(uint16)];
			f->ReadBytes(m_DoodadsIndexes, size);
		}
		else if (strcmp(fourcc, "MOBN") == 0)
		{
			assert1(m_Header.flags.FLAG_HAS_COLLISION);
		}
		else if (strcmp(fourcc, "MOBR") == 0)
		{
			assert1(m_Header.flags.FLAG_HAS_COLLISION);
		}
		else if (strcmp(fourcc, "MOCV") == 0) // Vertex colors
		{
			assert1(m_Header.flags.FLAG_HAS_VERTEX_COLORS);
			m_VertexColors = (uint32*)f->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MLIQ") == 0) // Liquid
		{
			WMO_Group_MLIQ liquidHeader;
			f->ReadBytes(&liquidHeader, sizeof(WMO_Group_MLIQ));

			enum liquid_basic_types
			{
				liquid_basic_types_water = 0,
				liquid_basic_types_ocean = 1,
				liquid_basic_types_magma = 2,
				liquid_basic_types_slime = 3,

				liquid_basic_types_MASK = 3,
			};

			if (m_Header.liquidType > 0)
			{
				Log::Green("WMO[%s]: Contain liquid! [%s]", m_ParentWMO->m_FileName.c_str(), DBC_LiquidType[m_Header.liquidType & 3]->Get_Name());
				Log::Green("WMO[%s]: LiquidType CHUNK = [%d], WMO = [%d]", m_ParentWMO->m_FileName.c_str(), liquidHeader.type, m_Header.liquidType);

				Liquid* liquid = new Liquid(liquidHeader.A, liquidHeader.B);
				liquid->CreateFromWMO(f, m_ParentWMO->m_Materials[liquidHeader.type], DBC_LiquidType[m_Header.liquidType & 3], m_Header.flags.FLAG_IS_INDOOR);

				m_LiquidInstance = new Liquid_Instance(liquid, liquidHeader.pos.toXZmY());
			}
		}
		else
		{
			Log::Info("WMO_Group[]: No implement group chunk %s [%d].", fourcc, size);
		}

		f->Seek(nextpos);
	}

	//

	WMOFog* wf = m_ParentWMO->m_Fogs[m_Header.m_Fogs[0]];
	if (wf->fogDef.largerRadius <= 0)
		fog = -1; // default outdoor fog..?
	else
		fog = m_Header.m_Fogs[0];

	//

	vec4* vertexColors = nullptr;
	if (m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		vertexColors = new vec4[m_VertexesCount];
	}

	// Converts
	for (uint32 i = 0; i < m_VertexesCount; i++)
	{
		m_Vertexes[i].toXZmY();
		m_Normals[i].toXZmY();
	}

    initLighting();

	uint32 bufferSize = 8 * sizeof(float);
	if (m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		bufferSize += sizeof(vec4);
	}

	// Vertex buffer
	R_Buffer* __vb = _Render->r.createVertexBuffer(m_VertexesCount * bufferSize, nullptr);
	__vb->updateBufferData(m_VertexesCount * 0 * sizeof(float), m_VertexesCount * sizeof(vec3), m_Vertexes);
	__vb->updateBufferData(m_VertexesCount * 3 * sizeof(float), m_VertexesCount * sizeof(vec2), m_TextureCoords);
	__vb->updateBufferData(m_VertexesCount * 5 * sizeof(float), m_VertexesCount * sizeof(vec3), m_Normals);
	if (m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		__vb->updateBufferData(m_VertexesCount * 8 * sizeof(float), m_VertexesCount * sizeof(vec4), vertexColors);
	}

	//

	__geom = _Render->r.beginCreatingGeometry(m_Header.flags.FLAG_HAS_VERTEX_COLORS ? _Render->Storage()->__layoutWMO_VC : _Render->Storage()->__layoutWMO);

	// Vertex params
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, m_VertexesCount * 0 * sizeof(float), 0);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, m_VertexesCount * 3 * sizeof(float), 0);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, m_VertexesCount * 5 * sizeof(float), 0);
	if (m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, m_VertexesCount * 8 * sizeof(float), 0);
	}

	// Index bufer
    R_Buffer* __ib = _Render->r.createIndexBuffer(m_IndicesCount * sizeof(uint16), m_Indices);
	__geom->setGeomIndexParams(__ib, R_IndexFormat::IDXFMT_16);

	// Finish
	__geom->finishCreatingGeometry();

	//

	if (m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		delete[] vertexColors;
	}
}

void WMOGroup::initLighting()
{
	if (m_Header.flags.FLAG_IS_INDOOR && m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		vec3 dirmin(1, 1, 1);
		float lenmin;
		int lmin;
#if CURRENT > CLASSIC
		for (uint32 i = 0; i < m_DoodadsIndexesCount; i++)
		{
			lenmin = 999999.0f * 999999.0f;
			lmin = 0;
			DoodadInstance* mi = m_ParentWMO->m_MDXInstances[m_DoodadsIndexes[i]];
			for (uint32 j = 0; j < m_ParentWMO->m_Header.nLights; j++)
			{
				WMOLight* l = m_ParentWMO->m_Lights[j];
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
#endif

		m_EnableOutdoorLights = false;
	}
	else
	{
		m_EnableOutdoorLights = true;
	}
}

//

void WMOGroup::Render()
{
	visible = false;

	BoundingBox aabb = m_Bounds;
	aabb.transform(_Pipeline->GetWorld());

	/*float dist = (aabb.Center - _Camera->Position).length();
	if (dist > _Config.culldistance + m_Bounds.Radius)
	{
		return false;
	}*/

	if (_CameraFrustum->_frustum.cullBox(aabb))
	{
		return;
	}

	visible = true;

	_Render->TechniquesMgr()->m_WMO_GeometryPass->Bind();
	_Render->TechniquesMgr()->m_WMO_GeometryPass->SetPVW();

	_Render->r.setGeometry(__geom);

	for (uint32 i = 0; i < m_WMOBatchIndexesCount; i++)
	{
		WMOBatch* batch = &m_WMOBatchIndexes[i];
		WMOMaterial* material = m_ParentWMO->m_Materials[batch->material_id];

		batch->__material.Set();

		//_Render->TechniquesMgr()->m_WMO_GeometryPass->SetDiffuseColor(fromARGB(material->GetDiffuseColor()));
		_Render->TechniquesMgr()->m_WMO_GeometryPass->SetHasMOCV(false);

		_Render->r.drawIndexed(PRIM_TRILIST, batch->indexStart, batch->indexCount, batch->vertexStart, (batch->vertexEnd - batch->vertexStart), false);
	}
}

bool WMOGroup::drawDoodads(uint32 _doodadSet)
{
	if (!visible)
	{
		return false;
	}

	// draw doodads
	for (uint32 i = 0; i < m_DoodadsIndexesCount; i++)
	{
		uint16 doodadIndex = m_DoodadsIndexes[i];

		if (m_ParentWMO->doodadsets[_doodadSet]->InSet(doodadIndex) || m_ParentWMO->doodadsets[0]->InSet(doodadIndex))
		{
			m_ParentWMO->m_MDXInstances[doodadIndex]->Render();
		}
	}

	return true;
}

void WMOGroup::setupFog()
{
	if (m_EnableOutdoorLights || fog == -1)
	{
		_World->EnvM()->SetFog();
	}
	else
	{
		m_ParentWMO->m_Fogs[fog]->setup();
	}
}