#pragma once

#include "M2_Types.h"

class CM2_Part_Bone
{
	friend class MDX;
	friend class CM2_Skin;
	friend class PlaneParticleEmitter;
	friend class SphereParticleEmitter;
	friend class RibbonEmitter;
public:
	void init(IFile* f, SM2_Bone& b, const vector<SM2_Loop>* global);
	void calcMatrix(CM2_Part_Bone* allbones, uint32 anim, uint32 time, uint32 globalTime);

	bool IsInterpolated() const
	{
		return trans.uses(0) || roll.uses(0) || scale.uses(0);
	}

	bool IsBillboard() const 
	{ 
		return
			m_Flags.spherical_billboard ||
			m_Flags.cylindrical_billboard_lock_x ||
			m_Flags.cylindrical_billboard_lock_y ||
			m_Flags.cylindrical_billboard_lock_z;
	}

private:
	SM2_Bone::Flags m_Flags;
	
	int16 parent;

	vec3 pivot, transPivot;

	Matrix4f m_TransformMatrix;
	Matrix4f m_RotationMatrix;

private:
	bool m_IsCalculated;

	M2_Animated<vec3> trans;
	M2_Animated<quat> roll;
	M2_Animated<vec3> scale;
};


