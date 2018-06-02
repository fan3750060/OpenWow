#include "stdafx.h"

// General
#include "RenderableUIObjectCollection.h"

struct RenderableUIObjectCompare
{
	bool operator() (const CRenderableUIObject* left, const CRenderableUIObject* right) const
	{
		return left->GetDrawOrder() < right->GetDrawOrder();
	}
};

bool CRenderableUIObjectCollection::RegisterObject(CRenderableUIObject* _uiObject, uint32 _DrawOrder)
{
	_uiObject->SetDrawOrder(_DrawOrder);
	m_Objects.push_back(_uiObject);
	m_ObjectsNeedSort = true;

	return true;
}

void CRenderableUIObjectCollection::UnregisterObject(CRenderableUIObject * _uiObject)
{
    m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), _uiObject), m_Objects.end());
	m_ObjectsNeedSort = true;
}

void CRenderableUIObjectCollection::RenderUI()
{
	if (m_ObjectsNeedSort)
	{
		std::sort(m_Objects.begin(), m_Objects.end(), RenderableUIObjectCompare());
		m_ObjectsNeedSort = false;
	}

	for (auto& it : m_Objects)
	{
		it->RenderUI();
	}
}
