#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_Animator.h"

CM2_Animator::CM2_Animator(const M2* _m2) :
	m_M2(_m2),
	m_IsLoop(false),
	m_IsPlayed(false),
	m_OnAnimationEnded(nullptr),
	animtime(0.0),
	m_CurrentTime(0)
{
	uint16 u = 0;
	for (auto i = DBÑ_AnimationData.Records().begin(); i != DBÑ_AnimationData.Records().end(); ++i)
	{
		DBÑ_AnimationDataRecord* record = (i->second);

		if (record->Get_ID() >= m_M2->m_Sequences.size())
		{
			break;
		}

		/*int16 indexIntoSequences = m_M2->m_Sequences[record->Get_ID()];
		if (indexIntoSequences == -1)
		{
			continue;
		}*/

		CM2_Animation* animation = new CM2_Animation(m_M2, m_M2->m_Sequences[record->Get_ID()], record->Get_ID(), record->Get_Name());
		m_Animations.insert(make_pair(record->Get_ID(), animation));
	}

	assert1(m_Animations.size() > 0);
	PlayAnimation();
}

CM2_Animator::~CM2_Animator()
{

}

void CM2_Animator::PlayAnimation(uint16 _id, bool _loop)
{
	m_IsLoop = _loop;

	if (_id != UINT16_MAX)
	{
		if ((m_Animations.find(_id) == m_Animations.end()))
		{
			Log::Error("ANIMATION [%d] not found!", _id);
			return;
		}

		m_CurrentAnimation = (m_Animations[_id]);
	}
	else
	{
		m_CurrentAnimation = m_Animations.begin()->second;
	}

	m_CurrentTime = m_CurrentAnimation->getStart();
	m_IsPlayed = false;
	animtime = 0;
}

void CM2_Animator::PrintList()
{
	for (auto& it : m_Animations)
	{
		Log::Warn("[%d] is [%s]", it.first, it.second->getName().c_str());
	}
}

void CM2_Animator::Update(double _time, double _dTime)
{
	if (m_IsPlayed)
	{
		return;
	}

	animtime += (_dTime * 1.0);
	m_CurrentTime = static_cast<uint32>(m_CurrentAnimation->getStart() + animtime);

	// Animation don't ended
	if (m_CurrentTime < m_CurrentAnimation->getEnd())
	{
		return;
	}

	// Ended!
	const CM2_Animation* nextAnim = m_CurrentAnimation->getNext();
	if (m_CurrentAnimation->getNext() != nullptr)
	{
		m_CurrentAnimation = nextAnim;
		m_CurrentTime = m_CurrentAnimation->getStart();
		m_IsPlayed = false;
		animtime = 0;
		return;
	}

	m_CurrentTime = m_CurrentAnimation->getEnd() - 1;
	m_IsPlayed = true;

	if (m_OnAnimationEnded != nullptr)
	{
		m_OnAnimationEnded->operator()();
	}

	if (m_IsLoop)
	{
		PlayAnimation(m_CurrentAnimation->getID());
	}
}

void CM2_Animator::setOnEndFunction(Function* _onEnd)
{
	if (m_OnAnimationEnded != nullptr)
	{
		delete m_OnAnimationEnded;
	}

	assert1(_onEnd);
	m_OnAnimationEnded = _onEnd;
}
