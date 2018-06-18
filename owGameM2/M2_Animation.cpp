#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_Animation.h"

CM2_Animation::CM2_Animation(const M2* _m2, const SM2_Sequence& _sequence, uint16 _sid, string _name) :
	m_ID(_sequence.__id),
	m_SID(_sid),
	m_Name(_name),
	m_Next(nullptr)
{
	if (_sequence.variationNext != -1)
	{
		m_Next = new CM2_Animation(_m2, _m2->m_Sequences[_sequence.variationNext], _sequence.variationNext, "???");
	}

	m_StartTimeStamp = _sequence.start_timestamp;
	m_EndTimeStamp = _sequence.end_timestamp;
}
