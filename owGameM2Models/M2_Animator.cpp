#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_Animator.h"

CM2_Animator::CM2_Animator(const M2* _m2) :
	m_M2(_m2)
{
	for (auto i = DBÑ_AnimationData.Records().begin(); i != DBÑ_AnimationData.Records().end(); ++i)
	{
		DBÑ_AnimationDataRecord* record = (i->second);

		if(record->Get_ID() >= m_M2->m_SequencesLookup.size())
		{
			break;
		}

		int16 indexIntoSequences = m_M2->m_SequencesLookup[record->Get_ID()];
		if (indexIntoSequences == -1)
		{
			continue;
		}

		CM2_Animation* animation = new CM2_Animation(m_M2, m_M2->m_Sequences[indexIntoSequences]);
		animation->m_SID = indexIntoSequences;

		m_Animations.insert(make_pair(record->Get_ID(), animation));
		//Log::Warn("Animation [%s][%d] added", record->Get_Name(), record->Get_ID());
	}

	assert1(m_Animations.size() > 0);
	m_CurrentAnimation = m_Animations[0];
}

CM2_Animator::~CM2_Animator() 
{
	
}

void CM2_Animator::PlayAnimation(uint16 _id)
{
	if (m_Animations.find(_id) == m_Animations.end())
	{
		Log::Error("ANIMATION [%d] not found!", _id);
		return;
	}

	m_CurrentAnimation = (m_Animations[_id]);
}
