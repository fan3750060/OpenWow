#pragma once

struct ADT_MODF
{
	uint32 nameIndex;
	uint32 uniqueId;
	vec3 position;
	vec3 rotation;
	CAaBox boundingBox;
	uint16 flags;
	uint16 doodadSetIndex;
	uint16 nameSet;
	uint16 unk0; 
};

#include "WMO.h"

class ADT_WMO_Instance : protected SceneNode
{
public:
	ADT_WMO_Instance(WMO* _wmoObject, ADT_MODF _placementInfo);
	~ADT_WMO_Instance();

	WMO* GetWMO() { return m_Object; }
	void Render();

private:
	void CalculateMatrix();

private:
	WMO*		m_Object;
	ADT_MODF	m_PlacementInfo;

public:	// Static
	static void reset();
private:
	static std::set<uint32> m_AlreadyDraw;
};