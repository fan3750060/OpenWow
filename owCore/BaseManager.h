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
	void SetPhase(SBaseManagerPhases _phase) override;
	SBaseManagerPhases GetPhase() override;

private:
	map<GUID, IManager*> m_Managers;
	SBaseManagerPhases m_CurrentPhase;
};

// Helpers

static inline IBaseManager* GetBaseManager()
{
	return CBaseManager::instance();
}

template<class T>
static inline void AddManager(IManager* _manager)
{
	GetBaseManager()->RegisterManager(__uuidof(T), _manager);
}

template<class T>
static inline void DelManager()
{
	GetBaseManager()->UnregisterManager(__uuidof(T));
}

template<class T>
static inline T* GetManager()
{
	return (T*)(GetBaseManager()->GetManager(__uuidof(T)));
}