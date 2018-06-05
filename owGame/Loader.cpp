#include "stdafx.h"

// General
#include "Loader.h"

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	wglMakeCurrent(_Render->hdc, _Render->glrc2);

	CLoader* loader = (CLoader*)lpParam;
	std::queue<ILoadable*>& lqueue = loader->getQueue();

	while (true)
	{
		WaitForSingleObject(loader->getEventID(), INFINITE);

		ILoadable* objectToLoad = lqueue.back();
		lqueue.pop();

		objectToLoad->Load();
		objectToLoad->SetLoaded();

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

void CLoader::AddToQueue(ILoadable* _item)
{
	/*if (m_Thread_Loader == nullptr)
	{
		m_Event_Add = CreateEventW(NULL, TRUE, TRUE, NULL);
		m_Thread_Loader = CreateThread(NULL, 0, &ThreadProc, this, NULL, NULL);
		//SetThreadPriority(m_TextureLoader, THREAD_PRIORITY_TIME_CRITICAL);
	}

	m_Queue.push(_item);
	SetEvent(m_Event_Add);*/
	_item->Load();
}
