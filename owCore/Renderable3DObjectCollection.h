#pragma once

#include "Renderable3DObject.h"

class CRenderable3DObjectCollection
{
public:
	bool RegisterObject(CRenderable3DObject* _uiObject, uint32 _DrawOrder = 0);
	void UnregisterObject(CRenderable3DObject* _uiObject);

	void Render3D(double t, double dt);

private:
	vector<CRenderable3DObject*>	m_Objects;
	bool					    m_ObjectsNeedSort;
};