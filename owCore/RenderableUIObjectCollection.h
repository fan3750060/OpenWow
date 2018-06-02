#pragma once

#include "RenderableUIObject.h"

class CRenderableUIObjectCollection
{
public:
	bool RegisterObject(CRenderableUIObject* _uiObject, uint32 _DrawOrder = 0);
	void UnregisterObject(CRenderableUIObject* _uiObject);

	void RenderUI();

private:
	vector<CRenderableUIObject*>   m_Objects;
	bool                          m_ObjectsNeedSort;
};