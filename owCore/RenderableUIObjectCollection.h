#pragma once

#include "RenderableUIObject.h"

class RenderableUIObjectCollection
{
public:
	bool RegisterObject(RenderableUIObject* _uiObject, uint32 _DrawOrder = 0);
	void UnregisterObject(RenderableUIObject* _uiObject);

	void RenderUI();

private:
	vector<RenderableUIObject*>   m_Objects;
	bool                          m_ObjectsNeedSort;
};