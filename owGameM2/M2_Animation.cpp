#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_Animation.h"

CM2_Animation::CM2_Animation(const M2* _m2, const SM2_Sequence& _sequence) :
	m_ID(_sequence.__id),
	m_Next(nullptr)
{
	if (_sequence.variationNext != -1)
	{
		m_Next = new CM2_Animation(_m2, (_m2->m_Sequences[_sequence.variationNext]));
	}

	m_StartTimeStamp = _sequence.start_timestamp;
	m_EndTimeStamp = _sequence.end_timestamp;
	m_Duration = _sequence.end_timestamp - _sequence.start_timestamp;

	if (m_Duration == 0) // BUG
	{
		m_Duration = 1;
	}
}
