#pragma once

#include "WMO_Headers.h"

#include "WMO_Group_Part_Batch.h"
#include "WMO_Liquid.h"

// FORWARD BEGIN
class WMO;
// FORWARD END

class WMO_Group : public CRefItem
{
public:
	WMO_Group(const WMO* _parentWMO, const uint32 m_GroupIndex, string _groupName, IFile* _groupFile);
	~WMO_Group();

	void CreateInsances(SceneNode* _parent);

	void Load();
	void initLighting();

	void Render();

	void setupFog();


public:
	const WMO*								m_ParentWMO;
	const string							m_GroupName;
	const SmartPtr<IFile>					m_F;
	WMO_Group_HeaderDef						m_Header;
	BoundingBox								m_Bounds;

public:
	int fog;
	bool m_EnableOutdoorLights;

	SmartGeomPtr __geom;

public:
	//-- Triangles --//
	uint32									m_MaterialsInfoCount;
	WMO_Group_MaterialDef*					m_MaterialsInfo; // MOPY chunk

	uint32									m_IndicesCount;
	uint16*									m_Indices;                  // MOVI chunk

	uint32									m_VertexesCount;
	vec3*									m_Vertexes;                   // MOVT chunk

	vec3*									m_Normals;                    // MONR chunk
	vec2*									m_TextureCoords;                  // MOTV chunk


	//-- Render bathes --//
	vector<SmartPtr<WMO_Group_Part_Batch>>	m_WMOBatchIndexes;      // MOBA chunk


	//-- Lights --//
	uint32									m_WMOLightsIndexesCount;
	uint16*									m_WMOLightsIndexes;     // MOLR chunk


	//-- Doodads references --// Break my engine logic
	//uint32 m_DoodadsIndexesCount;
	//uint16* m_DoodadsIndexes;       // MODR chunk


	//-- Collision --//
	// MOBN chunk
	// MOBR chunk

	//-- Vertex colors --//
	CBgra*							m_VertexColors;         // MOCV chunk

	//-- Liquid --//
	WMO_Group_MLIQDef				m_LiquidHeader;
	SmartPtr<CWMO_Liquid>			m_WMOLiqiud;

	const CGroupQuality& m_Quality;
};