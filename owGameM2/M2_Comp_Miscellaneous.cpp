#include "stdafx.h"

// Include
#include "M2_Builder.h"

// General
#include "M2_Comp_Miscellaneous.h"

CM2_Comp_Miscellaneous::CM2_Comp_Miscellaneous() :
	m_HasMisc(false)
{
}


CM2_Comp_Miscellaneous::~CM2_Comp_Miscellaneous()
{
	ERASE_VECTOR(m_Events);
	ERASE_VECTOR(m_Attachments);
	ERASE_VECTOR(m_Lights);
	ERASE_VECTOR(m_Cameras);

	ERASE_VECTOR(m_RibbonEmitters);
}

void CM2_Comp_Miscellaneous::calc(uint16 anim, uint32 time, uint32 globalTime, cmat4 _worldMat)
{
	for (auto& it : m_Lights)
	{
		it->setup(anim, time, globalTime);
	}

	for (auto& it : m_Cameras)
	{
		it->calc(time, globalTime);
	}

	for (auto& it : m_RibbonEmitters)
	{
		it->setup(anim, time, globalTime, _worldMat);
	}

	for (auto& it : particleSystems)
	{
		// random time distribution for teh win ..?
		//int pt = (animtime + (int)(tmax*particleSystems[i].tofs)) % tmax;
		it->setup(anim, time, globalTime);
	}
}

void CM2_Comp_Miscellaneous::render(cmat4 _worldMat)
{

	for (auto& it : m_Attachments)
	{
		it->render(_worldMat);
	}

	for (auto& it : m_RibbonEmitters)
	{
		it->Render(_worldMat);
	}

	for (auto& it : particleSystems)
	{
		it->draw();
	}
}
