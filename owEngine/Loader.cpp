#include "stdafx.h"

// General
#include "Loader.h"

CLoader::CLoader()
{
	for (int i = 0; i < c_PoolSize; i++)
	{
		std::future<void> futureObj = m_ThreadPromise[i].get_future();
		m_Thread_Loader[i] = std::thread(&CLoader::LoaderThread, this, std::move(futureObj));
		m_Thread_Loader[i].detach();
	}
	//m_ThreadPool = std::make_shared<ThreadPool>(4);
}

CLoader::~CLoader()
{
	for (int i = 0; i < c_PoolSize; i++)
	{
		m_ThreadPromise[i].set_value();
		if (m_Thread_Loader[i].joinable())
			m_Thread_Loader[i].join();
	}
}

void CLoader::AddToLoadQueue(std::shared_ptr<ILoadable> _item)
{
	m_QueueLoad.add(_item);
}

void CLoader::LoadAll()
{
	while (!m_QueueLoad.empty())
	{
		std::shared_ptr<ILoadable> obj = m_QueueLoad.peek();

		obj->Load();
		obj->setLoaded();

		m_QueueLoad.pop();
	}
}

void CLoader::AddToDeleteQueue(std::shared_ptr<ILoadable> _item)
{
	m_QueueDelete.add(_item);
}

void CLoader::DeleteAll()
{
	while (!m_QueueDelete.empty())
	{
		std::shared_ptr<ILoadable> obj = m_QueueDelete.peek();

		obj->Delete();
		obj.reset();

		m_QueueDelete.pop();
	}
}

void CLoader::LoaderThread(std::future<void> futureObj)
{
	while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		if (m_QueueLoad.empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			continue;
		}

		std::shared_ptr<ILoadable> objectToLoad;
		if (!m_QueueLoad.next(objectToLoad))
			continue;

		objectToLoad->Load();
		objectToLoad->setLoaded();
	}
}
