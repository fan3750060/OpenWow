#pragma once

#include "M2_Types.h"
#include "M2_Animation.h"

class CM2_Animator
{
public:
	CM2_Animator(const M2* _m2);
	~CM2_Animator();

	void PlayAnimation(uint16 _id);
	uint16 getSId() const { return m_CurrentAnimation->m_SID; }
	uint16 getCurrentId() const { return m_CurrentAnimation->getID(); }
	uint32 getCurrentTime(uint32 _global) const { return m_CurrentAnimation->getCurrentTime(_global); }

private:
	const M2*					m_M2;
	map<uint16, CM2_Animation*>	m_Animations;
	CM2_Animation*				m_CurrentAnimation;
};