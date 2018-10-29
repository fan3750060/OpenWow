#include "stdafx.h"

// General
#include "UpdatableObjectCollection.h"

// Additional
#include "BaseManager.h"

bool CUpdatableObjectCollection::RegisterObject(IUpdatable* _uiObject)
{
	assert1(_BaseManager->GetPhase() != Phase_Input && _BaseManager->GetPhase() != Phase_Update);
	m_Objects.push_back(_uiObject);

	return true;
}

void CUpdatableObjectCollection::UnregisterObject(IUpdatable * _uiObject)
{
	assert1(_BaseManager->GetPhase() != Phase_Input && _BaseManager->GetPhase() != Phase_Update);
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), _uiObject), m_Objects.end());
}

void CUpdatableObjectCollection::Update(CInput* _input, double _time, double _dTime)
{
	_BaseManager->SetPhase(Phase_Input);
	for (auto& it : m_Objects)
	{
		it->Input(_input, _time, _dTime);
	}

	//--

	_BaseManager->SetPhase(Phase_Update);
	for (auto& it : m_Objects)
	{
		it->Update(_time, _dTime);
	}

	_BaseManager->SetPhase(Phase_NONE);
}
