#pragma once

class CLoader : public ILoader
{
public:
	CLoader();
	~CLoader();

	void AddToQueue(ILoadable* _item) override;

public:
	std::queue<ILoadable*>& getQueue() { return m_Queue; }
	HANDLE& getThreadID() { return m_Thread_Loader; }
	HANDLE& getEventID() { return m_Event_Add; }

private:
	std::queue<ILoadable*> m_Queue;
	HANDLE m_Thread_Loader;
	HANDLE m_Event_Add;
};