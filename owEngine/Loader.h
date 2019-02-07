#pragma once

class CLoader : public ILoader
{
public:
	CLoader();
	~CLoader();

	void AddToLoadQueue(std::shared_ptr<ILoadable> _item) override;
	void LoadAll() override;

	void AddToDeleteQueue(std::shared_ptr<ILoadable> _item) override;
	void DeleteAll() override;

	void LoaderThread(std::future<void> futureObj);

private:
	const static uint32                    c_PoolSize = 16;

private:
	LockedQueue<std::shared_ptr<ILoadable>> m_QueueLoad;
	LockedQueue<std::shared_ptr<ILoadable>> m_QueueDelete;

	std::promise<void>					   m_ThreadPromise[c_PoolSize];
	std::thread                            m_Thread_Loader[c_PoolSize];

	std::shared_ptr<ThreadPool>            m_ThreadPool;


};