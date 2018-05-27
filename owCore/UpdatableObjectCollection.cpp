#include "stdafx.h"

// General
#include "UpdatableObjectCollection.h"

bool UpdatableObjectCollection::RegisterObject(IUpdatable* _uiObject)
{
	m_Objects.push_back(_uiObject);

	return true;
}

void UpdatableObjectCollection::UnregisterObject(IUpdatable * _uiObject)
{
    m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), _uiObject), m_Objects.end());
}

void UpdatableObjectCollection::Update(double _time, double _dTime)
{
    for (auto& it : m_Objects)
    {
        it->Input(_time, _dTime);
    }

	for (auto& it : m_Objects)
	{
		it->Update(_time, _dTime);
	}
}
