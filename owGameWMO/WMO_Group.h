#pragma once

#include "WMO_Group_Headers.h"

// Parts
#include "WMO_Liquid.h"
#include "WMO_Part_Portal.h"

#include "WMO_Group_Part_Batch.h"
#include "WMO_Group_Part_BSP_Node.h"

// FORWARD BEGIN
class WMO;
class CWMO_Group_Instance;
// FORWARD END

class WMO_Group : public CRefItem
{
public:
	WMO_Group(const WMO* _parentWMO, const uint32 m_GroupIndex, string _groupName, IFile* _groupFile);
	~WMO_Group();

	void CreateInsances(CWMO_Group_Instance* _parent) const;

	void Load();
	void initLighting();

	void Render(cmat4 _world) const;
	void RenderCollision(cmat4 _world) const;

public:
	const string							m_GroupName;
	const string							m_GroupDescription;
	const uint32							m_GroupIndex;
	IFile*									m_F;
	SWMO_Group_HeaderDef					m_Header;
	BoundingBox								m_Bounds;

	vector<CWMO_Part_Portal*>				m_Portals;

public:
	SmartGeomPtr							__geom;

public:
	//-- Triangles --//
	vec3 * dataFromMOVT;
	vector<SWMO_Group_MaterialDef>			m_MaterialsInfo;
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
	vector<uint16> collisionIndexes;
	SmartBufferPtr							VB_Collision;
	vector<CWMO_Group_Part_BSP_Node*>		m_CollisionNodes;

	//-- Liquid --//
	SWMO_Group_MLIQDef						m_LiquidHeader;
	SmartPtr<CWMO_Liquid>					m_WMOLiqiud;


	//--
	const WMO*								m_ParentWMO;
	const CGroupQuality&					m_Quality;
};