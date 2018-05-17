#include "stdafx.h"

// General(type
#include "Renderable3DObjectCollection.h"

vector<Renderable3DObject*> Renderable3DObjectCollection::m_Objects;
bool                        Renderable3DObjectCollection::m_ObjectsNeedSort;

struct Renderable3DObjectCompare
{
	bool operator() (const Renderable3DObject* left, const Renderable3DObject* right) const
	{
		return left->GetDrawOrder() < right->GetDrawOrder();
	}
};

bool Renderable3DObjectCollection::RegisterObject(Renderable3DObject* _uiObject)
{
	m_Objects.push_back(_uiObject);
	m_ObjectsNeedSort = true;

	return true;
}

void Renderable3DObjectCollection::UnregisterObject(Renderable3DObject * _uiObject)
{
    m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), _uiObject), m_Objects.end());
}

void Renderable3DObjectCollection::RenderUI()
{
	if (m_ObjectsNeedSort)
	{
		std::sort(m_Objects.begin(), m_Objects.end(), Renderable3DObjectCompare());
		m_ObjectsNeedSort = false;
	}

	for (auto it : m_Objects)
	{
		it->PreRender3D();
	}

	for (auto it : m_Objects)
	{
		it->Render3D();
	}

	for (auto it : m_Objects)
	{
		it->PostRender3D();
	}
}
