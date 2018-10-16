#pragma once

#include "Renderable3DObject.h"

struct Renderable3DObjectCompare
{
	bool operator() (const IRenderable3D* left, const IRenderable3D* right) const
	{
		return left->getDrawOrder() < right->getDrawOrder();
	}
};

class CRenderable3DObjectCollection
{
public:
	bool RegisterObject(IRenderable3D* _uiObject, uint32 _DrawOrder = 0);
	void UnregisterObject(IRenderable3D* _uiObject);

	void Render3D();

	void SetNeedSort() { m_ObjectsNeedSort = true; }

private:
	vector<IRenderable3D*>		m_Objects;
	bool					    m_ObjectsNeedSort;
};