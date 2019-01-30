#include "stdafx.h"

// General
#include "Loader.h"

DWORD WINAPI ThreadProcLoader(LPVOID lpParam)
{
	CLoader* loader = (CLoader*)lpParam;
	std::queue<ILoadable*>& lqueue = loader->getQueueLoad();

	//loader->getAdapter()->MakeMainContext();

	while (true)
	{
		WaitForSingleObject(loader->getEventID(), INFINITE);

		ILoadable* objectToLoad = lqueue.back();
		lqueue.pop();

		objectToLoad->Load();
		objectToLoad->setLoaded();

		ResetEvent(loader->getEventID());
	}

	ExitThread(0);
}

CLoader::CLoader() :
	m_Thread_Loader(nullptr)
{
	AddManager<ILoader>(this);
}

CLoader::~CLoader()
{
	ResetEvent(m_Event_Add);
	CloseHandle(m_Event_Add);

	TerminateThread(m_Thread_Loader, 1);
	CloseHandle(m_Thread_Loader);
}

void CLoader::AddToLoadQueue(ILoadable* _item)
{
	if (m_Thread_Loader == nullptr)
	{
		m_Event_Add = CreateEventW(NULL, TRUE, TRUE, NULL);
		m_Thread_Loader = CreateThread(NULL, 0, &ThreadProcLoader, this, NULL, NULL);
		//SetThreadPriority(m_TextureLoader, THREAD_PRIORITY_TIME_CRITICAL);
	}

	m_QueueLoad.push(_item);
	SetEvent(m_Event_Add);

	//_item->Load();
	//m_QueueLoad.push(_item);
}

void CLoader::LoadAll()
{
	while (!m_QueueLoad.empty())
	{
		ILoadable* obj = m_QueueLoad.front();

		obj->Load();
		obj->setLoaded();

		m_QueueLoad.pop();
	}
}

void CLoader::AddToDeleteQueue(ILoadable* _item)
{
	m_QueueDelete.push(_item);
}

void CLoader::DeleteAll()
{
	while (!m_QueueDelete.empty())
	{
		ILoadable* obj = m_QueueDelete.front();

		obj->Delete();
		delete obj;
		obj = nullptr;

		m_QueueDelete.pop();
	}
}
