#include "stdafx.h"

// General
#include "MDX.h"

// Additional
#include "EnvironmentManager.h"

void MDX::calcBones(uint32 _animationIndex, uint32 time, uint32 globalTime)
{
	for (uint32 i = 0; i < m_Header.bones.size; i++)
	{
		m_Bones[i].m_IsCalculated = false;
	}

	for (uint32 i = 0; i < m_Header.bones.size; i++)
	{
		m_Bones[i].calcMatrix(m_Bones.data(), _animationIndex, time, globalTime);
	}
}

void MDX::animate(uint32 _animationIndex, uint32 globalTime)
{
	m_AnimationIndex = _animationIndex;

	if (m_HasBones)
	{
		calcBones(_animationIndex, m_AnimationTime, globalTime);
	}

	/* Old animation without shader
	if (animGeometry)
	{
		M2Vertex* ov = m_OriginalVertexes;
		for (uint32 i = 0, k = 0; i < m_Header.vertices.size; ++i, ++ov)
		{
			vec3 vertex(0, 0, 0);
			vec3 normal(0, 0, 0);

			for (uint32 b = 0; b < 4; b++)
			{
				if (ov->bone_weights[b] > 0)
				{
					vertex += m_Bones[ov->bone_indices[b]].m_TransformMatrix * ov->pos * ((float)ov->bone_weights[b] / 255.0f);
					normal += m_Bones[ov->bone_indices[b]].m_RotationMatrix * ov->normal * ((float)ov->bone_weights[b] / 255.0f);
				}
			}

			m_Vertices[i] = vertex;
			m_Normals[i] = normal.normalized(); // shouldn't these be normal by default?
		}

		// Add sub-data
		_Render->r.updateBufferData(m_VBuffer, m_Header.vertices.size * 0 * sizeof(float), m_Header.vertices.size * sizeof(vec3), m_Vertices);
		_Render->r.updateBufferData(m_VBuffer, m_Header.vertices.size * 3 * sizeof(float), m_Header.vertices.size * sizeof(vec3), m_Normals);
	}*/

	for (uint32 i = 0; i < m_Header.lights.size; i++)
	{
		if (m_Lights[i].parent >= 0)
		{
			m_Lights[i].tpos = m_Bones[m_Lights[i].parent].m_TransformMatrix * m_Lights[i].pos;
			m_Lights[i].tdir = m_Bones[m_Lights[i].parent].m_RotationMatrix * m_Lights[i].dir;
		}
	}

#ifdef MDX_PARTICLES_ENABLE
	for (uint32 i = 0; i < m_Header.particle_emitters.size; i++)
	{
		// random time distribution for teh win ..?
		int pt = (animtime + (int)(tmax*particleSystems[i].tofs)) % tmax;
		particleSystems[i].setup(_animationIndex, pt);
	}

	for (uint32 i = 0; i < m_Header.ribbon_emitters.size; i++)
	{
		ribbons[i].setup(_animationIndex, animtime);
	}

#endif

	if (m_AnimTextures)
	{
		for (uint32 i = 0; i < m_Header.textureTransforms.size; i++)
		{
			m_TexturesTransform[i].calc(_animationIndex, m_AnimationTime, globalTime);
		}
	}

	if (m_HasMisc)
	{
		for (uint32 i = 0; i < m_Header.textureWeights.size; i++)
		{
			m_TextureWeights[i].calc(_animationIndex, m_AnimationTime, globalTime);
		}
	}
}