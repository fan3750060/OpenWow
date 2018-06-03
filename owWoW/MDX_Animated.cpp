#include "stdafx.h"

// General
#include "MDX.h"

// Additional
#include "EnvironmentManager.h"

void MDX::calcBones(uint32 _animationIndex, uint32 time, uint32 globalTime)
{
	for (uint32 i = 0; i < m_Header.bones.size; i++)
	{
		m_Bones[i].SetNeedCalculate();
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

	/*for (uint32 i = 0; i < m_Header.lights.size; i++)
	{
		if (m_Lights[i].getBoneIndex() >= 0)
		{
			m_Lights[i].tpos = m_Bones[m_Lights[i].getBoneIndex()].getTransformMatrix() * m_Lights[i].pos;
			m_Lights[i].tdir = m_Bones[m_Lights[i].getBoneIndex()].getRotateMatrix() * m_Lights[i].dir;
		}
	}*/


	for (auto it : m_RibbonEmitters)
	{
		it.setup(_animationIndex, m_AnimationTime, globalTime);
	}

#ifdef MDX_PARTICLES_ENABLE
	for (uint32 i = 0; i < m_Header.particle_emitters.size; i++)
	{
		// random time distribution for teh win ..?
		int pt = (animtime + (int)(tmax*particleSystems[i].tofs)) % tmax;
		particleSystems[i].setup(_animationIndex, pt);
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