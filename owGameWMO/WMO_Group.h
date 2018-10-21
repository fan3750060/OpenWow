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
class CWMO_Doodad_Instance;
// FORWARD END

class WMO_Group
{
public:
	WMO_Group(const WMO* _parentWMO, const uint32 m_GroupIndex, std::string _groupName, std::shared_ptr<IFile> _groupFile);
	virtual ~WMO_Group();

	void CreateInsances(std::shared_ptr<CWMO_Group_Instance> _parent) const;

	uint32 to_wmo_liquid(int x);

	void Load();
	void initLighting();

	void Render(cmat4 _world) const;
	void RenderCollision(cmat4 _world) const;

public:
	const std::string							m_GroupName;
	const std::string							m_GroupDescription;
	const uint32							m_GroupIndex;
	std::shared_ptr<IFile>					m_F;
	SWMO_Group_HeaderDef					m_Header;
	BoundingBox								m_Bounds;

	vector<CWMO_Part_Portal*>				m_Portals;

public:
	SharedMeshPtr							__geom;

public:
	//-- Triangles --//
	vec3 * dataFromMOVT;
	vector<SWMO_Group_MaterialDef>			m_MaterialsInfo;
	bool									m_IsMOCVExists;

	//-- Render bathes --//
	vector<WMO_Group_Part_Batch*>			m_WMOBatchIndexes;
	SWMO_Group_BatchDef*					moba;

	//-- Lights --//
	vector<uint16>							m_WMOLightsIndexes;
	C4Vec*									mocv;
	uint32									mocv_count;

	//-- Doodads references --//
	vector<uint16>							m_DoodadsPlacementIndexes;

	//-- Collision --//
	// MOBN chunk
	// MOBR chunk
	vector<uint16> collisionIndexes;
	SharedBufferPtr							VB_Collision;
	vector<CWMO_Group_Part_BSP_Node*>		m_CollisionNodes;

	//-- Liquid --//
	SWMO_Group_MLIQDef						m_LiquidHeader;
	std::shared_ptr<CWMO_Liquid>					m_WMOLiqiud;

	//--
	const WMO*								m_ParentWMO;
	const CGroupQuality&					m_Quality;
};