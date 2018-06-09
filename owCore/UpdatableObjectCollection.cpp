#include "stdafx.h"

// General
#include "UpdatableObjectCollection.h"

// Additional
#include "BaseManager.h"
#include "Perfomance.h"
bool CUpdatableObjectCollection::RegisterObject(IUpdatable* _uiObject)
{
	assert1(GetBaseManager()->GetPhase() != Phase_Input && GetBaseManager()->GetPhase() != Phase_Update);
	m_Objects.push_back(_uiObject);

	return true;
}

void CUpdatableObjectCollection::UnregisterObject(IUpdatable * _uiObject)
{
	assert1(GetBaseManager()->GetPhase() != Phase_Input && GetBaseManager()->GetPhase() != Phase_Update);
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), _uiObject), m_Objects.end());
}

void CUpdatableObjectCollection::Update(double _time, double _dTime)
{
	PERF_START(PERF_PHASE_INPUT);
	GetBaseManager()->SetPhase(Phase_Input);
	for (auto it : m_Objects)
	{
		it->Input(_time, _dTime);
	}
	PERF_STOP(PERF_PHASE_INPUT);

	//--

	PERF_START(PERF_PHASE_UPDATE);
	GetBaseManager()->SetPhase(Phase_Update);
	for (auto it : m_Objects)
	{
		it->Update(_time, _dTime);
	}
	PERF_STOP(PERF_PHASE_UPDATE);

	GetBaseManager()->SetPhase(Phase_NONE);
}
