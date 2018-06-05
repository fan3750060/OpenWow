#pragma once

#include "M2_Types.h"

class CM2_Part_Bone
{
public:
	CM2_Part_Bone(IFile* f, const SM2_Bone& _proto, cGlobalLoopSeq global);

	void calcMatrix(CM2_Part_Bone* allbones, uint32 anim, uint32 time, uint32 globalTime);

	bool IsInterpolated() const
	{
		return trans.uses() || roll.uses() || scale.uses();
	}

	bool IsBillboard() const
	{
		return
			m_Flags.spherical_billboard ||
			m_Flags.cylindrical_billboard_lock_x ||
			m_Flags.cylindrical_billboard_lock_y ||
			m_Flags.cylindrical_billboard_lock_z;
	}

	void SetNeedCalculate() { m_IsCalculated = false; }
	bool IsCalculated() const { return m_IsCalculated; }

	//

	int16 getParentBoneId() const { return parent; }
	cmat4 getTransformMatrix() const { return m_TransformMatrix; }
	cmat4 getRotateMatrix() const { return m_RotationMatrix; }
	cvec3 getPivot() const { return pivot; }

private:
	int32           m_Id; // Bones lookup table
	SM2_Bone::Flags m_Flags;

	int16			parent;
	uint16			submesh;

	vec3 pivot, transPivot;

	Matrix4f m_TransformMatrix;
	Matrix4f m_RotationMatrix;

private:
	bool m_IsCalculated;

	M2_Animated<vec3> trans;
	M2_Animated<quat> roll;
	M2_Animated<vec3> scale;
};


