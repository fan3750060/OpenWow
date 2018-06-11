#include "stdafx.h"

// Additional
#include "WMO.h"

// General
#include "WMO_Group.h"

WMO_Group::WMO_Group(const WMO* _parentWMO, const uint32 _groupIndex, string _groupName, IFile* _groupFile) :
	m_ParentWMO(_parentWMO),
	m_GroupName(_groupName),
	m_F(_groupFile),
	m_Quality(GetSettingsGroup<CGroupQuality>())
{
	m_MaterialsInfoCount = 0;
	m_MaterialsInfo = nullptr;

	m_Indices = nullptr;

	m_VertexesCount = 0;
	m_Vertexes = nullptr;

	m_Normals = nullptr;
	m_TextureCoords = nullptr;


	m_WMOLightsIndexesCount = 0;
	m_WMOLightsIndexes = nullptr;


	//m_DoodadsIndexesCount = 0;
	//m_DoodadsIndexes = nullptr;

	m_VertexColors = nullptr;

	m_WMOLiqiud = nullptr;
}

WMO_Group::~WMO_Group()
{
}

void WMO_Group::CreateInsances(SceneNode* _parent)
{
	if (m_WMOLiqiud != nullptr)
	{
		vec3 realPos = m_LiquidHeader.pos.toXZmY();
		realPos.y = 0.0f; // why they do this???

		new Liquid_Instance(_parent, m_WMOLiqiud, realPos);
	}
}

void WMO_Group::Load()
{
	char fourcc[5];
	uint32 size = 0;
	while (!m_F->IsEof())
	{
		memset(fourcc, 0, 4);
		size = 0;
		m_F->ReadBytes(fourcc, 4);
		m_F->ReadBytes(&size, 4);
		flipcc(fourcc);
		fourcc[4] = 0;
		if (size == 0)	continue;
		uint32_t nextpos = m_F->GetPos() + size;

		if (strcmp(fourcc, "MVER") == 0)
		{
			uint32 version;
			m_F->ReadBytes(&version, 4);
			assert1(version == 17);
		}
		else if (strcmp(fourcc, "MOGP") == 0)
		{
			nextpos = m_F->GetPos() + sizeof(WMO_Group_HeaderDef); // The MOGP chunk size will be way more than the header variables!

			m_F->ReadBytes(&m_Header, sizeof(WMO_Group_HeaderDef));

			// Bounds
			m_Bounds.set(m_Header.boundingBox.min, m_Header.boundingBox.max, true);
		}
		else if (strcmp(fourcc, "MOPY") == 0) // Material info for triangles
		{
			m_MaterialsInfoCount = size / sizeof(WMO_Group_MaterialDef);
			m_MaterialsInfo = (WMO_Group_MaterialDef*)m_F->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOVI") == 0) // Vertex indices for triangles
		{
			m_IndicesCount = size / sizeof(uint16);
			m_Indices = (uint16*)m_F->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOVT") == 0) // Vertices chunk.
		{
			m_VertexesCount = size / sizeof(vec3);
			m_Vertexes = (vec3*)m_F->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MONR") == 0) // Normals
		{
			m_Normals = (vec3*)m_F->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOTV") == 0) // R_Texture coordinates
		{
			m_TextureCoords = (vec2*)m_F->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOBA") == 0) // WMO_Group_Batch
		{
			for (uint32 i = 0; i < size / sizeof(WMO_Group_BatchDef); i++)
			{
				WMO_Group_BatchDef batchDef;
				m_F->ReadBytes(&batchDef, sizeof(WMO_Group_BatchDef));

				SmartPtr<WMO_Group_Part_Batch> batch = new WMO_Group_Part_Batch(m_ParentWMO, batchDef);
				m_WMOBatchIndexes.push_back(batch);
			}
		}
		else if (strcmp(fourcc, "MOLR") == 0) // Light references
		{
			assert1(m_Header.flags.FLAG_HAS_LIGHTS);
			m_WMOLightsIndexesCount = size / sizeof(uint16);
			m_WMOLightsIndexes = (uint16*)m_F->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MODR") == 0) // Doodad references
		{
			assert1(m_Header.flags.FLAG_HAS_DOODADS);
			//m_DoodadsIndexesCount = size / sizeof(uint16);
			//m_DoodadsIndexes = new uint16[size / sizeof(uint16)];
			//m_F->ReadBytes(m_DoodadsIndexes, size);
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
			assert1(size / sizeof(CBgra) == m_VertexesCount);

			m_VertexColors = (CBgra*)m_F->GetDataFromCurrent();

		}
		else if (strcmp(fourcc, "MLIQ") == 0) // Liquid
		{
			m_F->ReadBytes(&m_LiquidHeader, sizeof(WMO_Group_MLIQDef));

			if (m_Header.liquidType > 0)
			{
				Log::Green("WMO[%s]: LiquidType CHUNK = [%d]", m_ParentWMO->m_FileName.c_str(), m_LiquidHeader.type);

				assert1(m_Header.liquidType == 15);
				m_WMOLiqiud = new CWMO_Liquid(m_LiquidHeader.A, m_LiquidHeader.B);
				m_WMOLiqiud->CreateFromWMO(m_F, m_ParentWMO->m_Materials[m_LiquidHeader.type], DBC_LiquidType[m_Header.liquidType & 3], m_Header.flags.FLAG_IS_INDOOR);
			}
		}
		else
		{
			Log::Fatal("WMO_Group[]: No implement group chunk %s [%d].", fourcc, size);
		}
		m_F->Seek(nextpos);
	}

	//

	WMO_Part_Fog* wf = m_ParentWMO->m_Fogs[m_Header.m_Fogs[0]];
	if (wf->fogDef.largerRadius <= 0)
		fog = -1; // default outdoor fog..?
	else
		fog = m_Header.m_Fogs[0];

	//

	vec4* vertexColors = new vec4[m_VertexesCount];
	for (uint32 i = 0; i < m_VertexesCount; i++)
	{
		vertexColors[i] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	if (m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		for (uint32 i = 0; i < m_VertexesCount; i++)
		{
			vertexColors[i] = vec4
			(
				static_cast<float>(m_VertexColors[i].r) / 255.0f,
				static_cast<float>(m_VertexColors[i].g) / 255.0f,
				static_cast<float>(m_VertexColors[i].b) / 255.0f,
				static_cast<float>(m_VertexColors[i].a) / 255.0f
			);
		}
	}

	// Converts
	for (uint32 i = 0; i < m_VertexesCount; i++)
	{
		m_Vertexes[i] = m_Vertexes[i].toXZmY();
		m_Normals[i] = m_Normals[i].toXZmY();
	}

	initLighting();


	// Vertex buffer
	R_Buffer* __vb = _Render->r.createVertexBuffer(m_VertexesCount * 12 * sizeof(float), nullptr);
	__vb->updateBufferData(m_VertexesCount * 0 * sizeof(float), m_VertexesCount * sizeof(vec3), m_Vertexes);
	__vb->updateBufferData(m_VertexesCount * 3 * sizeof(float), m_VertexesCount * sizeof(vec2), m_TextureCoords);
	__vb->updateBufferData(m_VertexesCount * 5 * sizeof(float), m_VertexesCount * sizeof(vec3), m_Normals);
	__vb->updateBufferData(m_VertexesCount * 8 * sizeof(float), m_VertexesCount * sizeof(vec4), vertexColors);

	//

	__geom = _Render->r.beginCreatingGeometry(_Render->Storage()->__layoutWMO_VC);

	// Vertex params
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, m_VertexesCount * 0 * sizeof(float), 0);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, m_VertexesCount * 3 * sizeof(float), 0);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, m_VertexesCount * 5 * sizeof(float), 0);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, m_VertexesCount * 8 * sizeof(float), 0);

	// Index bufer
	R_Buffer* __ib = _Render->r.createIndexBuffer(m_IndicesCount * sizeof(uint16), m_Indices, false);
	__geom->setGeomIndexParams(__ib, R_IndexFormat::IDXFMT_16);

	// Finish
	__geom->finishCreatingGeometry();

	//

	delete[] vertexColors;
}

void WMO_Group::initLighting()
{
	if (m_Header.flags.FLAG_IS_INDOOR && m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		vec3 dirmin(1, 1, 1);
		float lenmin;
		/*int lmin;
		for (uint32 i = 0; i < m_DoodadsIndexesCount; i++)
		{
			lenmin = 999999.0f * 999999.0f;
			lmin = 0;
			DoodadInstance* mi = m_ParentWMO->m_M2Instances[m_DoodadsIndexes[i]];
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
		*/
		m_EnableOutdoorLights = false;
	}
	else
	{
		m_EnableOutdoorLights = true;
	}
}

//

void WMO_Group::Render()
{
	_Render->TechniquesMgr()->m_WMO_GeometryPass->Bind();
	_Render->TechniquesMgr()->m_WMO_GeometryPass->SetPVW();

	_Render->r.setGeometry(__geom);

	// Ambient color
	_Render->TechniquesMgr()->m_WMO_GeometryPass->SetHasMOCV(m_Quality.WMO_MOCV);

	// Ambient color
	_Render->TechniquesMgr()->m_WMO_GeometryPass->SetUseAmbColor(m_Quality.WMO_AmbColor);
	_Render->TechniquesMgr()->m_WMO_GeometryPass->SetAmbColor(m_ParentWMO->getAmbColor());
	
	for (auto it : m_WMOBatchIndexes)
	{
		it->Render();
	}

	_Render->TechniquesMgr()->m_WMO_GeometryPass->Unbind();
}

void WMO_Group::setupFog()
{
	if (m_EnableOutdoorLights || fog == -1)
	{
		//_World->EnvM()->SetFog();
	}
	else
	{
		m_ParentWMO->m_Fogs[fog]->setup();
	}
}