#pragma once

class CUpdatableObjectCollection
{
public:
	bool RegisterObject(IUpdatable* _uiObject);
	void UnregisterObject(IUpdatable* _uiObject);

	void Update(CInput* _input, double _time, double _dTime);

private:
	vector<IUpdatable*> m_Objects;
	IBaseManager* m_BaseManager;
};