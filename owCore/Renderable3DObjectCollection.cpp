#include "stdafx.h"

// General(type
#include "Renderable3DObjectCollection.h"

struct Renderable3DObjectCompare
{
	bool operator() (const Renderable3DObject* left, const Renderable3DObject* right) const
	{
		return left->GetDrawOrder() < right->GetDrawOrder();
	}
};



bool Renderable3DObjectCollection::RegisterObject(Renderable3DObject* _uiObject, uint32 _DrawOrder)
{
	_uiObject->SetDrawOrder(_DrawOrder);
	m_Objects.push_back(_uiObject);
	m_ObjectsNeedSort = true;

	return true;
}

void Renderable3DObjectCollection::UnregisterObject(Renderable3DObject* _uiObject)
{
    m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), _uiObject), m_Objects.end());
	m_ObjectsNeedSort = true;
}

void Renderable3DObjectCollection::Render3D(double t, double dt)
{
	if (m_ObjectsNeedSort)
	{
		std::sort(m_Objects.begin(), m_Objects.end(), Renderable3DObjectCompare());
		m_ObjectsNeedSort = false;
	}

	for (auto& it : m_Objects)
	{
		it->PreRender3D(t, dt);
	}

	for (auto& it : m_Objects)
	{
		if (it->IsVisible())
		{
			it->Render3D();
		}
	}

	for (auto& it : m_Objects)
	{
		if (it->IsVisible())
		{
			it->PostRender3D();
		}
	}
}
