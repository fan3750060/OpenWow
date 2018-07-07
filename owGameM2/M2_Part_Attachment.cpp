#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_Part_Attachment.h"

CM2_Part_Attachment::CM2_Part_Attachment(const M2* _parentM2, IFile* f, const SM2_Attachment& _proto, cGlobalLoopSeq global)
{
	assert1(_proto.id < M2_AttachmentType::Count);
	m_Type = (M2_AttachmentType::List)_proto.id;
	m_Bone = _parentM2->getBone(_proto.bone);
	m_Position = _proto.position;
	m_IsAnimateAttached.init(_proto.animate_attached, f, global);
}
