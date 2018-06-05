#pragma once

#include "Renderable3DObject.h"

class CRenderable3DObjectCollection
{
public:
	bool RegisterObject(IRenderable3D* _uiObject, uint32 _DrawOrder = 0);
	void UnregisterObject(IRenderable3D* _uiObject);

	void Render3D(double _time, double _dTime);

private:
	vector<IRenderable3D*>		m_Objects;
	bool					    m_ObjectsNeedSort;
};