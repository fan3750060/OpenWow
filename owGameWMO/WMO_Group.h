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

	void Render(cmat4 _worldMatrix);

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
	vector<WMO_Group_MaterialDef>			m_MaterialsInfo;

	bool									m_IsMOCVExists;

	//-- Render bathes --//
	vector<SmartPtr<WMO_Group_Part_Batch>>	m_WMOBatchIndexes;

	//-- Lights --//
	vector<uint16>							m_WMOLightsIndexes;

	//-- Doodads references --// 
	vector<uint16>							m_DoodadsIndexes;

	//-- Collision --//
	// MOBN chunk
	// MOBR chunk

	//-- Liquid --//
	WMO_Group_MLIQDef				m_LiquidHeader;
	SmartPtr<CWMO_Liquid>			m_WMOLiqiud;

	const CGroupQuality& m_Quality;
};