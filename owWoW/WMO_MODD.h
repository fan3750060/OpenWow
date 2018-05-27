#pragma once

struct DoodadPlacementInfo
{
	struct
	{
		uint32 nameIndex : 24;          // reference offset into MODN
		uint32 flag_AcceptProjTex : 1;
		uint32 : 7;                     // unused as of 7.0.1.20994
	} flags;

	vec3 position;				// (X,Z,-Y)
	C4ImQuaternion orientation;	// (X, Y, Z, W)
	float scale;				// scale factor
	CBgra color;				// (B,G,R,A) diffuse lighting color, used in place of global diffuse from DBCs
};

#include "MDX.h"

class WMO_MODD : public SceneNode
{
public:
	WMO_MODD(IFile* f);

	void SetModel(MDX* m);
	MDX* GetMDX() { return m_Object; }

	void Render();

	uint32 GetNameIndex() { return m_NameIndex; }

private:
	void CalculateMatrix();

private:
	SmartMDXPtr	m_Object;
	uint32		m_NameIndex;
};