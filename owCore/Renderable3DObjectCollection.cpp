#include "stdafx.h"

// General(type
#include "Renderable3DObjectCollection.h"

// Additional
#include "BaseManager.h"
#include "Perfomance.h"





bool CRenderable3DObjectCollection::RegisterObject(IRenderable3D* _uiObject, uint32 _DrawOrder)
{
	if (_uiObject->GetDrawOrder() < _DrawOrder || _DrawOrder != 0)
	{
		_uiObject->SetDrawOrder(_DrawOrder);
	}

	m_Objects.push_back(_uiObject);
	m_ObjectsNeedSort = true;

	return true;
}

void CRenderable3DObjectCollection::UnregisterObject(IRenderable3D* _uiObject)
{
    m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), _uiObject), m_Objects.end());
	m_ObjectsNeedSort = true;

	assert1(GetBaseManager()->GetPhase() != Phase_Pre3D && GetBaseManager()->GetPhase() != Phase_3D && GetBaseManager()->GetPhase() != Phase_Post3D);
}

void CRenderable3DObjectCollection::Render3D()
{
	if (m_ObjectsNeedSort)
	{
		std::sort(m_Objects.begin(), m_Objects.end(), Renderable3DObjectCompare());
		m_ObjectsNeedSort = false;
	}

	PERF_START(PERF_PHASE_PRE3D);
	GetBaseManager()->SetPhase(Phase_Pre3D);
	for (auto& it : m_Objects)
	{
		it->PreRender3D();
	}
	PERF_STOP(PERF_PHASE_PRE3D);

	//--

	PERF_START(PERF_PHASE_3D);
	GetBaseManager()->SetPhase(Phase_3D);
	for (auto& it : m_Objects)
	{
		if (it->IsVisible())
		{
			it->Render3D();
		}
	}
	PERF_STOP(PERF_PHASE_3D);

	//--

	PERF_START(PERF_PHASE_POST3D);
	GetBaseManager()->SetPhase(Phase_Post3D);
	for (auto& it : m_Objects)
	{
		if (it->IsVisible())
		{
			it->PostRender3D();
		}
	}
	PERF_STOP(PERF_PHASE_POST3D);

	GetBaseManager()->SetPhase(Phase_NONE);
}
