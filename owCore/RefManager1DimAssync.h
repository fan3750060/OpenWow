#pragma once

#include "MapAssync.h"
#define DISABLE_ASSYNC

#ifndef DISABLE_ASSYNC
extern HANDLE m_Thread_Loader;
extern HANDLE m_Event_Add;
DWORD WINAPI ThreadProc(LPVOID lpParam);
#endif


template <class T>
class CRefManager1DimAssync
{
public:
	CRefManager1DimAssync();
	~CRefManager1DimAssync();

	shared_ptr<T> Add(cstring name);

	bool Exists(cstring name) const;

	void Delete(cstring name);
	void Delete(shared_ptr<T> item);

	// Getters
	shared_ptr<T> GetItemByName(cstring name) const;
	string GetNameByItem(shared_ptr<T> item) const;

	// Console
	void PrintAllInfo();

public:
	virtual shared_ptr<T> CreateAction(cstring name);
	virtual void LoadAction(string name, shared_ptr<T>& item);
	virtual bool DeleteAction(cstring name);
	virtual void MakeContext();

public:
	map<string, shared_ptr<T>> objects;

#ifndef DISABLE_ASSYNC
public:
	//static HANDLE m_Thread_Loader;
	//static HANDLE m_Event_Add;
	MapAssync<string, T*> m_ObjectsToLoad;
#endif
};

#include "RefManager1DimAssync.inl"