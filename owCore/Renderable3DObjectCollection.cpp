#include "stdafx.h"

// General(type
#include "Renderable3DObjectCollection.h"

// Additional
#include "BaseManager.h"

bool CRenderable3DObjectCollection::RegisterObject(IRenderable3D* _uiObject, uint32 _DrawOrder)
{
	if (_uiObject->getDrawOrder() < _DrawOrder || _DrawOrder != 0)
	{
		_uiObject->setDrawOrder(_DrawOrder);
	}

	m_Objects.push_back(_uiObject);
	m_ObjectsNeedSort = true;

	return true;
}

void CRenderable3DObjectCollection::UnregisterObject(IRenderable3D* _uiObject)
{
    m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), _uiObject), m_Objects.end());
	m_ObjectsNeedSort = true;

	_ASSERT(_BaseManager->GetPhase() != Phase_Pre3D && _BaseManager->GetPhase() != Phase_3D && _BaseManager->GetPhase() != Phase_Post3D);
}

void CRenderable3DObjectCollection::Render3D(IPerfomance* _perfomance)
{
	if (m_ObjectsNeedSort)
	{
		std::sort(m_Objects.begin(), m_Objects.end(), Renderable3DObjectCompare());
		m_ObjectsNeedSort = false;
	}

	_perfomance->Start(PERF_PHASE_PRE3D);
	{
		_BaseManager->SetPhase(Phase_Pre3D);
		for (auto& it : m_Objects)
		{
			it->setVisible(it->PreRender3D());
			_perfomance->Inc(PERF_PHASE_PRE3D);
		}
	}
	_perfomance->Stop(PERF_PHASE_PRE3D);

	//--

	_perfomance->Start(PERF_PHASE_3D);
	{
		_BaseManager->SetPhase(Phase_3D);
		for (auto& it : m_Objects)
		{
			if (it->isVisible())
			{
				it->Render3D();
				_perfomance->Inc(PERF_PHASE_3D);
			}
		}
	}
	_perfomance->Stop(PERF_PHASE_3D);

	//--

	_perfomance->Start(PERF_PHASE_POST3D);
	{
		_BaseManager->SetPhase(Phase_Post3D);
		for (auto& it : m_Objects)
		{
			if (it->isVisible())
			{
				it->PostRender3D();
				_perfomance->Inc(PERF_PHASE_POST3D);
			}
		}
	}
	_perfomance->Stop(PERF_PHASE_POST3D);

	_BaseManager->SetPhase(Phase_NONE);
}
