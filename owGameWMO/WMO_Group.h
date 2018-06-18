#pragma once

#include "WMO_Headers.h"

#include "WMO_Group_Part_Batch.h"
#include "WMO_Liquid.h"
#include "WMO_Part_Portal.h"

// FORWARD BEGIN
class WMO;
class WMO_Doodad_Instance;
// FORWARD END

struct MODD_PlacementInfo
{
	const WMO_Doodad_PlacementInfo** infos;
	uint32 count;
};

class WMO_Group : public CRefItem
{
public:
	WMO_Group(const WMO* _parentWMO, const uint32 m_GroupIndex, string _groupName, IFile* _groupFile);
	~WMO_Group();

	void CreateInsances(SceneNode* _parent);

	void Load();
	void initLighting();

	void Render(cmat4 _worldMatrix, const WMO_Doodad_SetInfo& _doodadSet);

	bool isPointInside(cvec3 _point);

public:
	const WMO*								m_ParentWMO;
	const string							m_GroupName;
	const uint32							m_GroupIndex;
	const SmartPtr<IFile>					m_F;
	WMO_Group_HeaderDef						m_Header;
	BoundingBox								m_Bounds;
	bool									m_PortalsVis;
	bool									m_Calculated;
	vector<CWMO_Part_Portal*>				m_Portals;

public:
	SmartGeomPtr							__geom;

public:
	//-- Triangles --//
	vector<WMO_Group_MaterialDef>			m_MaterialsInfo;
	bool									m_IsMOCVExists;

	//-- Render bathes --//
	vector<SmartPtr<WMO_Group_Part_Batch>>	m_WMOBatchIndexes;

	//-- Lights --//
	vector<uint16>							m_WMOLightsIndexes;

	//-- Doodads references --//
	vector<uint16>							m_DoodadsPlacementIndexes;

	//-- Collision --//
	// MOBN chunk
	// MOBR chunk

	//-- Liquid --//
	WMO_Group_MLIQDef						m_LiquidHeader;
	SmartPtr<CWMO_Liquid>					m_WMOLiqiud;

	const CGroupQuality&					m_Quality;
};