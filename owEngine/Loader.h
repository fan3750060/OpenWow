#pragma once

class CLoader : public ILoader
{
public:
	CLoader();
	~CLoader();

	void AddToLoadQueue(ILoadable* _item) override;
	void LoadAll() override;

	void AddToDeleteQueue(ILoadable* _item) override;
	void DeleteAll() override;

public:
	std::queue<ILoadable*>& getQueueLoad() { return m_QueueLoad; }
	std::queue<ILoadable*>& getQueueDelete() { return m_QueueDelete; }
	HANDLE& getThreadID() { return m_Thread_Loader; }
	HANDLE& getEventID() { return m_Event_Add; }
	HANDLE& getEventLoadedID() { return m_Event_Loaded; }

private:
	std::queue<ILoadable*> m_QueueLoad;
	std::queue<ILoadable*> m_QueueDelete;
	HANDLE m_Thread_Loader;
	HANDLE m_Event_Add;
	HANDLE m_Event_Loaded;
};