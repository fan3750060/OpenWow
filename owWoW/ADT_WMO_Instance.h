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

class ADT_WMO_Instance : protected SceneNode, public Renderable3DObject
{
public:
	ADT_WMO_Instance(WMO* _wmoObject, ADT_MODF _placementInfo);
	~ADT_WMO_Instance();

	// Renderable3DObject
	void PreRender3D(double t, double dt);
	void Render3D();

	inline WMO* GetWMO() { return m_Object; }

private:
	WMO*	m_Object;
	uint32	m_UniqueId;
	uint16	m_DoodadSetIndex;

public:	// Static
	static void reset();
private:
	static std::set<uint32> m_AlreadyDraw;
};