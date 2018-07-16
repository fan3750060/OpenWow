#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_Part_Event.h"

CM2_Part_Event::CM2_Part_Event(const M2* _parentM2, IFile* f, const SM2_Event& _proto, cGlobalLoopSeq global)
{
	memcpy(m_ID, _proto.identifier, 4);
	m_Data = _proto.data;
	m_Bone = _parentM2->getSkeleton()->getBoneLookup(_proto.bone);
	m_Position = _proto.position;
	//m_IsEnabled.init(_proto.enabled, f, global);
}
