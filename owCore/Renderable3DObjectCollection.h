#pragma once

#include "Renderable3DObject.h"

class Renderable3DObjectCollection
{
public:
	bool RegisterObject(Renderable3DObject* _uiObject, uint32 _DrawOrder = 0);
	void UnregisterObject(Renderable3DObject* _uiObject);

	void Render3D(double t, double dt);

private:
	vector<Renderable3DObject*>	m_Objects;
	bool					    m_ObjectsNeedSort;
};