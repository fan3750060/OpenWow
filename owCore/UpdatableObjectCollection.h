#pragma once

#include "UpdatableObject.h"

class UpdatableObjectCollection
{
public:
	bool RegisterObject(UpdatableObject* _uiObject);
	void UnregisterObject(UpdatableObject* _uiObject);

	void Update(double _Time, double _deltaTime);

private:
	vector<UpdatableObject*> m_Objects;
};