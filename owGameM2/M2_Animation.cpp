#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_Animation.h"

CM2_Animation::CM2_Animation(const M2* _m2, uint16 _animID, string _name, uint16 indexIntoSeq, const SM2_Sequence& _sequence) :
	m_AnimID(_animID),
	m_Name(_name),
	m_SequenceIndex(indexIntoSeq),
	m_Next(nullptr)
{
	int16 nextIndex = _sequence.variationNext;
	if (nextIndex != -1)
	{
		m_Next = new CM2_Animation(_m2, _animID, (_name + "" + to_string(nextIndex)), nextIndex, _m2->m_Sequences[nextIndex]);
	}

#if (VERSION == VERSION_Vanila)
	m_StartTimeStamp = _sequence.start_timestamp;
	m_EndTimeStamp = _sequence.end_timestamp;
#else
	m_StartTimeStamp = 0;
	m_EndTimeStamp = _sequence.duration;
#endif
}
