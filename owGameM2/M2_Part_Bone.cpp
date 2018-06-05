#include "stdafx.h"

// General
#include "M2_Part_Bone.h"

CM2_Part_Bone::CM2_Part_Bone(IFile* f, const SM2_Bone& _proto, cGlobalLoopSeq global)
{
	m_Id = _proto.key_bone_id;
	m_Flags = _proto.flags;
	parent = _proto.parent_bone;
	
	trans.init(_proto.translation, f, global, Fix_XZmY);
	roll.init(_proto.rotation, f, global, Fix_XZmYW);
	scale.init(_proto.scale, f, global, Fix_XZY);

	pivot = _proto.pivot.toXZmY();
}

void CM2_Part_Bone::calcMatrix(CM2_Part_Bone* allbones, uint32 anim, uint32 time, uint32 globalTime)
{
	if (m_IsCalculated)
	{
		return;
	}

	mat4 m;

	if (roll.uses() || scale.uses() || trans.uses() || (IsBillboard()))
	{
		m.translate(pivot);

		if (trans.uses())
		{
			m.translate(trans.getValue(anim, time, globalTime));
		}

		if (roll.uses())
		{
			quat q = roll.getValue(anim, time, globalTime);
			m.rotate(q);

			if (parent >= 0)
			{
				m_RotationMatrix = allbones[parent].m_RotationMatrix * mat4::RotMat(q);
			}
			else
			{
				m_RotationMatrix = mat4::RotMat(q);
			}
		}

		if (scale.uses())
		{
			m.scale(scale.getValue(anim, time, globalTime));
		}

		if (IsBillboard())
		{
			if (parent != -1)
			{
				if (allbones[parent].IsBillboard())
				{
					goto xxx;
				}
			}

			mat4 worldPrevMatrix = m * _PipelineGlobal->GetWorld();

			bool shitMatrix = false;
			float scaleX = vec3(worldPrevMatrix.x[0], worldPrevMatrix.x[1], worldPrevMatrix.x[2]).length();
			float scaleY = vec3(worldPrevMatrix.x[4], worldPrevMatrix.x[5], worldPrevMatrix.x[6]).length();
			float scaleZ = vec3(worldPrevMatrix.x[8], worldPrevMatrix.x[9], worldPrevMatrix.x[10]).length();

			mat4 matrix = _PipelineGlobal->GetView();
			if (!shitMatrix)
			{
				matrix *= worldPrevMatrix;
			}

			// Set vectors
			vec3 vUp;
			vec3 vRight = (vec3(matrix.c[0][0], matrix.c[1][0], matrix.c[2][0]) / scaleX) * -1.0f;

			// Calc billboard type
			if (m_Flags.spherical_billboard)
			{
				vUp = vec3(matrix.c[0][1], matrix.c[1][1], matrix.c[2][1]) / scaleY;
			}
			else if (m_Flags.cylindrical_billboard_lock_x)
			{
				vUp = vec3(1, 0, 0);
			}
			else if(m_Flags.cylindrical_billboard_lock_y)
			{
				vUp = vec3(0, 0, 1);
			}
			else if(m_Flags.cylindrical_billboard_lock_z)
			{
				vUp = vec3(0, 1, 0);
			}

			//vec3 vForward = vec3(matrix.c[0][2], matrix.c[1][2], matrix.c[2][2]) / scaleZ;
			//m.c[0][0] = vForward.x;
			//m.c[0][1] = vForward.y;
			//m.c[0][2] = vForward.z;

			m.c[1][0] = vUp.x;
			m.c[1][1] = vUp.y;
			m.c[1][2] = vUp.z;

			m.c[2][0] = vRight.x;
			m.c[2][1] = vRight.y;
			m.c[2][2] = vRight.z;
		}
xxx:
		m.translate(pivot * -1.0f);
	}

	if (parent >= 0)
	{
		allbones[parent].calcMatrix(allbones, anim, time, globalTime);
		m_TransformMatrix = allbones[parent].m_TransformMatrix * m;
	}
	else
	{
		m_TransformMatrix = m;
	}

	transPivot = m_TransformMatrix * pivot;

	m_IsCalculated = true;
}
