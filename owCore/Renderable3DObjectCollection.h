#pragma once

#include "Renderable3DObject.h"

class Renderable3DObjectCollection
{
public:
	static bool RegisterObject(Renderable3DObject* _uiObject);
	static void UnregisterObject(Renderable3DObject* _uiObject);

	static void RenderUI();

private:
	static vector<Renderable3DObject*>   m_Objects;
	static bool                          m_ObjectsNeedSort;
};