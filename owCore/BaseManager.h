#pragma once

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

// Helpers

template<class T>
static inline void AddManager(IManager* _manager)
{
	CBaseManager::instance()->RegisterManager(__uuidof(T), _manager);
}

template<class T>
static inline void DelManager()
{
	CBaseManager::instance()->UnregisterManager(__uuidof(T));
}

template<class T>
static inline T* GetManager()
{
	return (T*)(CBaseManager::instance()->GetManager(__uuidof(T)));
}