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

	T* Add(cstring name);

	bool Exists(cstring name) const;

	void Delete(cstring name);
	void Delete(T* item);

	// Getters
	T* GetItemByName(cstring name) const;
	string GetNameByItem(T* item) const;

	// Console
	void PrintAllInfo();

public:
	virtual T* CreateAction(cstring name);
	virtual void LoadAction(string name, T*& item);
	virtual bool DeleteAction(cstring name);
	virtual void MakeContext();

public:
	map<string, T*> objects;

#ifndef DISABLE_ASSYNC
public:
	//static HANDLE m_Thread_Loader;
	//static HANDLE m_Event_Add;
	MapAssync<string, T*> m_ObjectsToLoad;
#endif
};

#include "RefManager1DimAssync.inl"