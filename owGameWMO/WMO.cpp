#include "stdafx.h"

// General
#include "WMO.h"

struct WMO_GroupInfoDef
{
	WMOGroupFlags flags;
	CAaBox bounding_box;
	int32 nameoffset;                                   // name in MOGN chunk (-1 for no name)
};

WMO::WMO(cstring name) :
	m_FileName(name),
	m_Loaded(false),

	m_TexturesNames(nullptr),
	m_GroupsNames(nullptr),
	m_Skybox_Filename(nullptr),
	m_M2Filenames(nullptr)

{}

WMO::~WMO()
{
	Log::Info("WMO[%s]: Unloading...", m_FileName.c_str());

	//

	if (m_TexturesNames) delete[] m_TexturesNames;
	if (m_GroupsNames) delete[] m_GroupsNames;
	if (m_Skybox_Filename) delete[] m_Skybox_Filename;
}

void WMO::CreateInsances(SceneNode* _parent)
{
	for (auto& it : m_Groups)
	{
		it->CreateInsances(_parent);
	}

	for (auto& it : m_M2PlacementInfos)
	{
		SmartPtr<M2> mdx = (M2*)GetManager<IM2Manager>()->Add(m_M2Filenames + it.flags.nameIndex);
		new WMO_MODD_Instance(_parent, mdx, it);
	}
}

bool WMO::Load()
{
	UniquePtr<IFile> f = GetManager<IFilesManager>()->Open(m_FileName);
	if (f == nullptr)
	{
		return false;
	}

	Log::Info("WMO[%s]: Loading...", m_FileName.c_str());

	char fourcc[5];
	uint32 size;
	while (!f->IsEof())
	{
		memset(fourcc, 0, 4);
		f->ReadBytes(fourcc, 4);
		flipcc(fourcc);
		fourcc[4] = 0;
		size = 0;
		f->ReadBytes(&size, 4);
		if (size == 0) continue;
		uint32_t nextpos = f->GetPos() + size;

		if (strcmp(fourcc, "MVER") == 0)                    // Version
		{
			uint32 version;
			f->ReadBytes(&version, 4);
			assert1(version == 17);
		}
		else if (strcmp(fourcc, "MOHD") == 0)               // Header
		{
			f->ReadBytes(&m_Header, sizeof(WMO_HeaderDef));

			m_Bounds.set(m_Header.bounding_box.min, m_Header.bounding_box.max, true);
		}
		else if (strcmp(fourcc, "MOTX") == 0)               // List of m_DiffuseTextures (BLP Files) used in this map object.
		{
			m_TexturesNames = new char[size + 1];
			f->ReadBytes(m_TexturesNames, size);
			m_TexturesNames[size] = 0x00;
		}
		else if (strcmp(fourcc, "MOMT") == 0)               // Materials used in this map object, 64 bytes per texture (BLP file), nMaterials entries.
		{
			for (uint32 i = 0; i < m_Header.nTextures; i++)
			{
				WMO_MaterialDef material;
				f->ReadBytes(&material, sizeof(WMO_MaterialDef));

				SmartPtr<WMO_Part_Material> _mat = new WMO_Part_Material(this, material);
				m_Materials.push_back(_mat);
			}
		}
		else if (strcmp(fourcc, "MOGN") == 0)              // List of group names for the m_Groups in this map object.
		{
			m_GroupsNames = new char[size + 1];
			f->ReadBytes(m_GroupsNames, size);
			m_GroupsNames[size] = 0x00;
		}
		else if (strcmp(fourcc, "MOGI") == 0)
		{
			for (uint32 i = 0; i < m_Header.nGroups; i++)
			{
				WMO_GroupInfoDef groupInfo;
				f->ReadBytes(&groupInfo, sizeof(WMO_GroupInfoDef));

				char temp[256];
				strcpy_s(temp, m_FileName.c_str());
				temp[m_FileName.length() - 4] = 0;

				char fname[256];
				sprintf_s(fname, "%s_%03d.wmo", temp, i);
				SmartPtr<IFile> groupFile = GetManager<IFilesManager>()->Open(fname);

				string groupName = groupFile->Name();
				if (groupInfo.nameoffset > 0)
				{
					groupName = string(m_GroupsNames + groupInfo.nameoffset);
				}

				WMO_Group* group = new WMO_Group(this, i, groupName, groupFile);
				m_Groups.push_back(group);
			}
		}
		else if (strcmp(fourcc, "MOSB") == 0) // Skybox. 
		{
			if (size > 4)
			{
				m_Skybox_Filename = new char[size + 1];
				f->ReadBytes(m_Skybox_Filename, size);
				m_Skybox_Filename[size] = 0x00;
				Log::Error("WMO[%s]: Skybox [%s]", m_FileName.c_str(), m_Skybox_Filename);
				//m_Skybox = _World->LoadMDX(m_Skybox_Filename);
			}
		}
		else if (strcmp(fourcc, "MOPV") == 0)
		{
			for (uint32 i = 0; i < size / sizeof(vec3); i++)
			{
				vec3 portalVertex;
				f->ReadBytes(&portalVertex, sizeof(vec3));
				m_PortalVertices.push_back(portalVertex.toXZmY());
			}
		}
		else if (strcmp(fourcc, "MOPT") == 0)
		{
			assert1((size / sizeof(WMO_PortalDef)) == m_Header.nPortals);

			for (uint32 i = 0; i < size / sizeof(WMO_PortalDef); i++)
			{
				WMO_PortalDef portalInformation;
				f->ReadBytes(&portalInformation, sizeof(WMO_PortalDef));
				m_PortalInformation.push_back(portalInformation);
			}
		}
		else if (strcmp(fourcc, "MOPR") == 0)
		{
			for (uint32 i = 0; i < size / sizeof(WMO_PortalReferencesDef); i++)
			{
				WMO_PortalReferencesDef portalReference;
				f->ReadBytes(&portalReference, sizeof(WMO_PortalReferencesDef));
				m_PortalReferences.push_back(portalReference);
			}
		}
		else if (strcmp(fourcc, "MOVV") == 0)
		{
			for (uint32 i = 0; i < size / sizeof(vec3); i++)
			{
				vec3 visibleVertex;
				f->ReadBytes(&visibleVertex, sizeof(vec3));
				m_VisibleBlockVertices.push_back(visibleVertex.toXZmY());
			}
		}
		else if (strcmp(fourcc, "MOVB") == 0)
		{
			for (uint32 i = 0; i < size / sizeof(WMO_VisibleBlockListDef); i++)
			{
				WMO_VisibleBlockListDef visibleBlockList;
				f->ReadBytes(&visibleBlockList, sizeof(WMO_VisibleBlockListDef));
				m_VisibleBlockList.push_back(visibleBlockList);
			}

		}
		else if (strcmp(fourcc, "MOLT") == 0)
		{
			assert1((size / sizeof(WMO_LightDef)) == m_Header.nLights);

			for (uint32 i = 0; i < m_Header.nLights; i++)
			{
				WMO_LightDef lightDef;
				f->ReadBytes(&lightDef, sizeof(WMO_LightDef));

				SmartPtr<WMO_Part_Light> _wmoLight = new WMO_Part_Light(lightDef);
				m_Lights.push_back(_wmoLight);
			}
		}
		else if (strcmp(fourcc, "MODS") == 0)
		{
			assert1((size / sizeof(WMO_MODD_SetInfo)) == m_Header.nDoodadSets);

			for (uint32 i = 0; i < m_Header.nDoodadSets; i++)
			{
				WMO_MODD_SetInfo dds;
				f->ReadBytes(&dds, sizeof(WMO_MODD_SetInfo));
				m_M2SetInfos.push_back(dds);
			}
		}
		else if (strcmp(fourcc, "MODN") == 0) // List of filenames for M2 (mdx) models that appear in this WMO.
		{
			m_M2Filenames = (char*)f->GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MODD") == 0) // Information for doodad instances. 40 bytes per doodad instance, nDoodads entries.
		{
			m_Header.nDoodadDefs = (size / sizeof(WMO_MODD_PlacementInfo)); // HACK! INCORRECT SIZE
			assert1((size / sizeof(WMO_MODD_PlacementInfo)) == m_Header.nDoodadDefs);

			for (uint32 i = 0; i < m_Header.nDoodadDefs; i++)
			{
				WMO_MODD_PlacementInfo placementInfo;
				f->ReadBytes(&placementInfo, sizeof(WMO_MODD_PlacementInfo));
				m_M2PlacementInfos.push_back(placementInfo);
			}
		}
		else if (strcmp(fourcc, "MFOG") == 0)
		{
			for (uint32 i = 0; i < (size / sizeof(WMO_FogDef)); i++)
			{
				WMO_FogDef fogDef;
				f->ReadBytes(&fogDef, sizeof(WMO_FogDef));

				SmartPtr<WMO_Part_Fog> fog = new WMO_Part_Fog(fogDef);
				m_Fogs.push_back(fog);
			}
		}
		else if (strcmp(fourcc, "MCVP") == 0)
		{
			//Convex Volume Planes.Contains blocks of floating - point numbers.
			// C4Plane convex_volume_planes[];   // normal points out
		}
		else
		{
			Log::Fatal("WMO[%s]: No implement wmo chunk [%s] [%d].", m_FileName.c_str(), fourcc, size);
		}

		f->Seek(nextpos);
	}

	// Init m_Groups
	for (auto it = m_Groups.begin(); it != m_Groups.end(); ++it)
	{
		(*it)->Load();
	}

	m_Loaded = true;

	return true;
}

//

void WMO::Update(double _time, double _dTime)
{
	if (!m_Loaded || m_Skybox == nullptr)
	{
		return;
	}

	//m_Skybox->Update(_time, _dTime);
}

bool WMO::Render(uint32 _doodadSet)
{
	if (!m_Loaded)
	{
		return false;
	}

	PERF_START(PERF_MAP_MODELS_WMOs_GEOMETRY);
	for (auto it = m_Groups.begin(); it != m_Groups.end(); ++it)
	{
		(*it)->Render();
	}
	PERF_STOP(PERF_MAP_MODELS_WMOs_GEOMETRY);

	// Debug geometry
	/*{
		_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);
		_Render->TechniquesMgr()->m_Debug_GeometryPass->Bind();

		#ifdef _DEBUG
		DEBUG_DrawLightPlaceHolders();
		DEBUG_DrawFogPositions();
		DEBUG_DrawMainBoundingBox();
		DEBUG_DrawMainBoundingSphere();
		DEBUG_DrawBoundingBoxes();
		DEBUG_DrawPortalsRelations();
		DEBUG_DrawPortals();
		#endif

		_Render->TechniquesMgr()->m_Debug_GeometryPass->Unbind();
		_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);
	}*/

	return true;
}

bool WMO::drawSkybox()
{
	if (!m_Loaded)
	{
		return false;
	}

	if (m_Skybox == nullptr)
	{
		return false;
	}

	_Pipeline->Clear();
	_Pipeline->Translate(_Camera->Position);
	_Pipeline->Scale(2.0f);

	m_Skybox->Render();

	//_World->EnvM()->m_HasSky = true;

	return true;
}

//

void WMO::DEBUG_DrawLightPlaceHolders()
{
	//glDisable(GL_CULL_FACE);
	/*glColor4f(1, 1, 1, 1);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < m_Lights.size(); i++)
	{
		glColor4fv(m_Lights[i]->fcolor);

		glVertex3fv(m_Lights[i]->lightDef.pos);
		glVertex3fv(m_Lights[i]->lightDef.pos + vec3(-0.5f, 1, 0));
		glVertex3fv(m_Lights[i]->lightDef.pos + vec3(0.5f, 1, 0));
	}
	glEnd();*/

	//glEnable(GL_CULL_FACE);
}

void WMO::DEBUG_DrawFogPositions()
{
	//glDisable(GL_TEXTURE_2D);

	/*glColor4f(1, 1, 1, 1);

	for (uint32_t i = 0; i < m_Fogs.size(); i++)
	{
		WMO_Part_Fog* fog = m_Fogs[i];

		glBegin(GL_LINE_LOOP);
		glVertex3fv(fog->fogDef.position);
		glVertex3fv(fog->fogDef.position + vec3(fog->fogDef.smallerRadius, 5, -fog->fogDef.largerRadius));
		glVertex3fv(fog->fogDef.position + vec3(fog->fogDef.smallerRadius, 5, fog->fogDef.largerRadius));
		glVertex3fv(fog->fogDef.position + vec3(-fog->fogDef.smallerRadius, 5, fog->fogDef.largerRadius));
		glVertex3fv(fog->fogDef.position + vec3(-fog->fogDef.smallerRadius, 5, -fog->fogDef.largerRadius));
		glEnd();
	}

	glColor4f(1, 1, 1, 1);*/

	//glEnable(GL_TEXTURE_2D);
}

void WMO::DEBUG_DrawMainBoundingBox()
{
	/*_Render->r.setGeometry(_Render->Storage()->_cubeGeo);

	_Pipeline->Push(); // Save world matrix
	{
		_Pipeline->Translate(m_Bounds.Min);
		_Pipeline->Scale(m_Bounds.Max - m_Bounds.Min);

		_Render->TechniquesMgr()->m_Debug_GeometryPass->SetPVW();
	}
	_Pipeline->Pop();

	_Render->TechniquesMgr()->m_Debug_GeometryPass->SetColor4(vec4(1.0f, 1.0f, 1.0f, 0.7f));

	_Render->r.drawIndexed(PRIM_TRILIST, 0, 36, 0, 8);*/
}

void WMO::DEBUG_DrawMainBoundingSphere()
{
	/*_Render->r.setGeometry(_Render->Storage()->_sphereGeo);

	_Pipeline->Push(); // Save world matrix
	{
		//_Pipeline->Translate(m_Bounds.Center);
		_Pipeline->Scale(m_Bounds.Radius);

		_Render->TechniquesMgr()->m_Debug_GeometryPass->SetPVW();
	}
	_Pipeline->Pop();

	_Render->TechniquesMgr()->m_Debug_GeometryPass->SetColor4(vec4(1.0f, 1.0f, 1.0f, 0.7f));

	_Render->r.drawIndexed(PRIM_TRILIST, 0, 128 * 3, 0, 126);*/
}

void WMO::DEBUG_DrawBoundingBoxes()
{
	_Render->r.setGeometry(_Render->Storage()->_cubeGeo);

	for (int i = 0; i < m_Header.nGroups; i++)
	{
		WMO_Group* g = m_Groups[i];
		float fc[2] = { 1, 0 };

		//_Pipeline->Push(); // Save world matrix
		{
			_Pipeline->Translate(g->m_Bounds.Min);
			_Pipeline->Scale(g->m_Bounds.Max - g->m_Bounds.Min);

			_Render->TechniquesMgr()->m_Debug_GeometryPass->SetPVW();
		}
		//_Pipeline->Pop();

		_Render->TechniquesMgr()->m_Debug_GeometryPass->SetColor4(vec4(fc[i % 2], fc[(i / 2) % 2], fc[(i / 3) % 2], 0.7f));



		_Render->r.drawIndexed(PRIM_TRILIST, 0, 36, 0, 8, false);
	}
}

void WMO::DEBUG_DrawPortalsRelations()
{
	for (uint32_t i = 0; i < m_Header.nPortals; i++)
	{
		/*WMO_PortalDef* portalInformation = m_PortalInformation[i];
		WMO_PortalReferencesDef* portalReference = m_PortalReferences[i];
		vec3 pv = m_PortalVertices[portalReference->portalIndex];

		if (portalReference->side > 0)
		{
			_Render->TechniquesMgr()->m_Debug_GeometryPass->SetColor4(vec4(1.0f, 0.0f, 0.0f, 0.8f));
		}
		else
		{
			_Render->TechniquesMgr()->m_Debug_GeometryPass->SetColor4(vec4(0.0f, 0.0f, 1.0f, 0.8f));
		}

		vec3 pc;
		for (uint32 j = portalInformation->startVertex; j < portalInformation->count; j++)
		{
			pc += m_PortalVertices[j];
		}
		pc *= 0.25f;
		vec3 gc = (m_Groups[portalReference->groupIndex]->m_Bounds.Min + m_Groups[portalReference->groupIndex]->m_Bounds.Max) * 0.5f;

		//

		vector<vec3> verts;
		verts.push_back(pc);
		verts.push_back(gc);

		//

		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vec3), verts.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_LINES, 0, verts.size());

		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &buffer);*/
	}
}

void WMO::DEBUG_DrawPortals()
{
	_Render->TechniquesMgr()->m_Debug_GeometryPass->SetColor4(vec4(0.0f, 1.0f, 0.0f, 0.5f));

	for (uint32 i = 0; i < m_Header.nPortals; i++)
	{
		/*WMO_PortalDef* portalInformation = m_PortalInformation[i];

		vector<vec3> verts;
		for (uint32 j = portalInformation->startVertex; j < portalInformation->count; j++)
		{
			verts.push_back(m_PortalVertices[j]);
		}

		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vec3), verts.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, verts.size());

		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &buffer);*/
	}
}