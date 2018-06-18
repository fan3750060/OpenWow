#pragma once

#include "M2_Types.h"

class CM2_Animation
{
public:
	CM2_Animation(const M2* _m2, const SM2_Sequence& _sequence, uint16 _sid, string _name);

	uint16 getID() const { return m_ID; } // In sequence
	uint16 getSID() const { return m_SID; } // In table
	string getName() const { return m_Name; }
	const CM2_Animation* getNext() const { return m_Next; }


	uint32 getStart() const { return m_StartTimeStamp; }
	uint32 getEnd() const { return m_EndTimeStamp; }

private:
	const uint16			m_ID;
	const uint16			m_SID;
	const string			m_Name;
	const CM2_Animation*	m_Next;
	uint32					m_StartTimeStamp;
	uint32					m_EndTimeStamp;
};