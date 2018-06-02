#pragma once

#include "MDX_Headers.h"

class MDX_Part_Bone
{
	friend class MDX;
	friend class Model_Skin;
	friend class PlaneParticleEmitter;
	friend class SphereParticleEmitter;
	friend class RibbonEmitter;
public:
	void init(IFile* f, M2CompBone& b, uint32* global);
	void calcMatrix(MDX_Part_Bone* allbones, uint32 anim, uint32 time);

private:
	
		struct BillboardType
		{
			enum List
			{
				BILLBOARD_DISABLED,
				BILLBOARD_SPHERICAL,
				BILLBOARD_CYLINDRICAL_X,
				BILLBOARD_CYLINDRICAL_Y,
				BILLBOARD_CYLINDRICAL_Z
			};
		};
	
private:
	vec3 pivot, transPivot;
	int16 parent;

	uint8 m_BillboardType;

	Matrix4f m_TransformMatrix;
	Matrix4f m_RotationMatrix;

private:
	bool m_IsCalculated;

	Animated<vec3> trans;
	Animated<quat> roll;
	Animated<vec3> scale;
};


