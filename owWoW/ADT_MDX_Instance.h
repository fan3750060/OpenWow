#pragma once

struct ADT_MDDF
{
	uint32 nameIndex;
	uint32 uniqueId;
	vec3 position;
	vec3 rotation;
	uint16 scale;
    struct
    {
        uint16 Flag_Biodome : 1;
        uint16 Flag_Shrubbery : 1;
        uint16 : 14;
    } flags;
};

#include "MDX.h"

class ADT_MDX_Instance : protected SceneNode
{
public:
	ADT_MDX_Instance(MDX* _mdxObject, ADT_MDDF _placementInfo);
	~ADT_MDX_Instance();

    MDX* GetMDX() { return m_Object; }
	void Render();

private:
	void CalculateMatrix();

public: 
	MDX*		m_Object;
	ADT_MDDF	m_PlacementInfo;

public:	// Static
	static void reset();

private:
	static std::set<uint32> m_AlreadyDraw;
};