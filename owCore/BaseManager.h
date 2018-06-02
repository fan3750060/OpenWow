#pragma once

#include "SmartPtr.h"

inline bool operator<(const GUID& lhs, const GUID& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(GUID)) < 0;
}

class CBaseManager : public IBaseManager
{
	CLASS_INSTANCE(CBaseManager)
public:
	CBaseManager();
	~CBaseManager();

	// IBaseManager
	void RegisterManager(GUID _type, IManager* _manager) override;
	void UnregisterManager(GUID _type) override;
	IManager* GetManager(GUID _type) override;

private:
	map<GUID, IManager*> m_Managers;
};

#define _BaseManager CBaseManager::instance()

template<class T>
static inline void AddManager(IManager* _manager)
{
	_BaseManager->RegisterManager(__uuidof(T), _manager);
}

template<class T>
static inline void DelManager()
{
	_BaseManager->UnregisterManager(__uuidof(T));
}

template<class T>
static inline T* GetManager()
{
	return (T*)(_BaseManager->GetManager(__uuidof(T)));
}