#include "stdafx.h"

// General
#include "RenderableUIObjectCollection.h"

CRenderableUIObjectCollection::~CRenderableUIObjectCollection()
{
	ERASE_VECTOR(m_Objects);
}

bool CRenderableUIObjectCollection::RegisterObject(IRenderableUI* _uiObject, uint32 _DrawOrder)
{
	m_Objects.push_back(new CRenderableUIObject(_uiObject, _DrawOrder));
	std::sort(m_Objects.begin(), m_Objects.end(), CRenderableUIObjectCompare());

	return true;
}

void CRenderableUIObjectCollection::UnregisterObject(IRenderableUI * _Object)
{
	for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it)
	{
		if ((*it)->getInputListener() == _Object)
		{
			m_Objects.erase(it);
			break;
		}
	}
}

void CRenderableUIObjectCollection::RenderUI()
{
	for (auto& it : m_Objects)
	{
		it->RenderUI();
	}
}
