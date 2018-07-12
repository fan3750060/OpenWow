#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_Part_Bone.h"

CM2_Part_Bone::CM2_Part_Bone(IFile* f, const SM2_Bone& _proto, cGlobalLoopSeq global, vector<IFile*>* animfiles) :
	m_ParentBone(nullptr)
{
	m_Id = _proto.key_bone_id;
	m_Flags = _proto.flags;
	m_ParentBoneID = _proto.parent_bone;
	
	trans.init(_proto.translation, f, global, Fix_XZmY, animfiles);
	roll.init(_proto.rotation, f, global, Fix_XZmYW, animfiles);
	scale.init(_proto.scale, f, global, Fix_XZY, animfiles);

	pivot = _proto.pivot.toXZmY();
}

void CM2_Part_Bone::setParentBone(const M2* _parentM2)
{
	if (m_ParentBoneID != -1)
	{
		m_ParentBone = _parentM2->m_Bones[m_ParentBoneID];
	}
}

void CM2_Part_Bone::calcMatrix(uint16 anim, uint32 time, uint32 globalTime)
{
	if (m_IsCalculated)
	{
		return;
	}

	if (m_ParentBone != nullptr)
	{
		m_ParentBone->calcMatrix(anim, time, globalTime);
	}

	mat4 m;
	if (roll.uses(anim) || scale.uses(anim) || trans.uses(anim))
	{
		m.translate(pivot);

		if (trans.uses(anim))
		{
			m.translate(trans.getValue(anim, time, globalTime));
		}

		if (roll.uses(anim))
		{
			quat q = roll.getValue(anim, time, globalTime);
			m.rotate(q);

			if (m_ParentBone != nullptr)
			{
				m_RotationMatrix = m_ParentBone->m_RotationMatrix * mat4::RotMat(q);
			}
			else
			{
				m_RotationMatrix = mat4::RotMat(q);
			}
		}

		if (scale.uses(anim))
		{
			m.scale(scale.getValue(anim, time, globalTime));
		}

		m.translate(pivot * -1.0f);
	}

	if (m_ParentBone != nullptr)
	{
		assert1(m_ParentBone->IsCalculated());
		m_TransformMatrix = m_ParentBone->getTransformMatrix() * m;
	}
	else
	{
		m_TransformMatrix = m;
	}

	transPivot = m_TransformMatrix * pivot;

	m_IsCalculated = true;
}

void CM2_Part_Bone::calcBillboard(cmat4 _worldMatrix)
{
	if (IsBillboard())
	{
		m_TransformMatrix.translate(pivot);
		{
			mat4 W = _worldMatrix * m_TransformMatrix;
			mat4 VW = _Render->getCamera()->getViewMat() * W;

			// Set vectors default
			vec3 worldScale = W.extractScale();
			vec3 vRight   = vec3(VW.c[0][0], VW.c[1][0], VW.c[2][0]) / worldScale.x;
			vec3 vUp      = vec3(VW.c[0][1], VW.c[1][1], VW.c[2][1]) / worldScale.y;
			vec3 vForward = vec3(VW.c[0][2], VW.c[1][2], VW.c[2][2]) / worldScale.z;
			vRight *= -1.0f;

			if (m_Flags.cylindrical_billboard_lock_x)
			{
				vUp = vec3(VW.c[0][1], 0, 0);
			}
			else if (m_Flags.cylindrical_billboard_lock_y)
			{
				vUp = vec3(0, VW.c[1][1], 0);
			}
			else if (m_Flags.cylindrical_billboard_lock_z)
			{
				vUp = vec3(0, 0, VW.c[2][1]);
			}

			m_TransformMatrix.c[0][0] = vForward.x;
			m_TransformMatrix.c[0][1] = vForward.y;
			m_TransformMatrix.c[0][2] = vForward.z;

			m_TransformMatrix.c[1][0] = vUp.x;
			m_TransformMatrix.c[1][1] = vUp.y;
			m_TransformMatrix.c[1][2] = vUp.z;

			m_TransformMatrix.c[2][0] = vRight.x;
			m_TransformMatrix.c[2][1] = vRight.y;
			m_TransformMatrix.c[2][2] = vRight.z;
		}
		m_TransformMatrix.translate(pivot * -1.0f);
	}
}
