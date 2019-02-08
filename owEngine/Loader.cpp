#include "stdafx.h"

// General
#include "Loader.h"

// Additional
#include "SceneNode3D.h"

CLoader::CLoader()
{
	for (int i = 0; i < c_PoolSize; i++)
	{
		std::future<void> futureObj = m_ThreadPromise[i].get_future();
		m_Thread_Loader[i] = std::thread(&CLoader::LoaderThread, this, std::move(futureObj));
		m_Thread_Loader[i].detach();
	}

	std::future<void> futureObj = m_Thread_Sorter_Promise.get_future();
	m_Thread_Sorter = std::thread(&CLoader::SorterThread, this, std::move(futureObj));
	m_Thread_Sorter.detach();
}

CLoader::~CLoader()
{
	for (int i = 0; i < c_PoolSize; i++)
	{
		m_ThreadPromise[i].set_value();
		if (m_Thread_Loader[i].joinable())
			m_Thread_Loader[i].join();
	}

	m_Thread_Sorter_Promise.set_value();
	if (m_Thread_Sorter.joinable())
		m_Thread_Sorter.join();
}

void CLoader::AddToLoadQueue(std::shared_ptr<ILoadable> _item)
{
	_item->PreLoad();
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


void CLoader::SetCamera(std::shared_ptr<Camera> _camera)
{
	m_Camera = _camera;
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

void CLoader::SorterThread(std::future<void> futureObj)
{
	while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		if (m_QueueLoad.empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			continue;
		}

		m_QueueLoad.lock();
		m_QueueLoad.getList().sort(sortFunctor(m_Camera));
		m_QueueLoad.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

bool CLoader::sortFunctor::operator()(const std::shared_ptr<ILoadable>& first, const std::shared_ptr<ILoadable>& second)
{
	if (first->getPriority() == second->getPriority())
	{
		if (camera)
		{
			std::shared_ptr<SceneNode3D> node3DFirst = std::dynamic_pointer_cast<SceneNode3D, ILoadable>(first);
			std::shared_ptr<SceneNode3D> node3DSecond = std::dynamic_pointer_cast<SceneNode3D, ILoadable>(second);

			
			bool cullFirst = !camera->GetFrustum().cullSphere(node3DFirst->GetTranslation(), 1.0f);
			bool cullSecond = !camera->GetFrustum().cullSphere(node3DSecond->GetTranslation(), 1.0f);
			
			if (cullFirst && !cullSecond)
				return true;

			if (!cullFirst && cullSecond)
				return false;

			float distToCamera2DFirst = glm::length(Fix_X0Z(camera->GetTranslation()) - node3DFirst->GetTranslation());
			float distToCamera2DSecond = glm::length(Fix_X0Z(camera->GetTranslation()) - node3DSecond->GetTranslation());
			return distToCamera2DFirst < distToCamera2DSecond;
		}
	}

	return first->getPriority() < second->getPriority();
}