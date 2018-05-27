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

class ADT_MDX_Instance : protected SceneNode, public Renderable3DObject
{
public:
	ADT_MDX_Instance(MDX* _mdxObject, ADT_MDDF _placementInfo);
	~ADT_MDX_Instance();

	// Renderable3DObject
	void PreRender3D(double t, double dt);
	void Render3D();

    inline MDX* GetMDX() { return m_Object; }

public: 
	MDX*	m_Object;
	uint32	m_UniqueId;

public:	// Static
	static void reset();

private:
	static std::set<uint32> m_AlreadyDraw;
};