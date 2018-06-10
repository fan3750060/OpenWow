#pragma once

#include "M2_Types.h"

class CM2_Animation
{
public:
	CM2_Animation(const M2* _m2, const SM2_Sequence& _sequence);

	uint16 getID() const { return m_ID; }
	uint32 getCurrentTime(uint32 _global) const { return (_global % m_Duration) + m_StartTimeStamp; }

	uint16			m_Number;
	uint16			m_SID;

private:
	const uint16			m_ID;
	const CM2_Animation*	m_Next;
	uint32					m_StartTimeStamp;
	uint32					m_EndTimeStamp;
	uint32					m_Duration;
};