#pragma once

inline bool operator<(const GUID& lhs, const GUID& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(GUID)) < 0;
}

class CBaseManager : public IBaseManager
{
public:
	CBaseManager();
	~CBaseManager();

	// IBaseManager
	void RegisterManager(GUID _type, std::shared_ptr<IManager> _manager) override;
	void UnregisterManager(GUID _type) override;
	std::shared_ptr<IManager> GetManager(GUID _type) override;
	void SetPhase(SBaseManagerPhases _phase) override;
	SBaseManagerPhases GetPhase() override;

private:
	std::map<GUID, std::shared_ptr<IManager>> m_Managers;
	SBaseManagerPhases m_CurrentPhase;
};

extern std::shared_ptr<CBaseManager> _BaseManager;

// Helpers

template<class T>
static inline void AddManager(std::shared_ptr<IManager> _manager)
{
	_BaseManager->RegisterManager(__uuidof(T), _manager);
}

template<class T>
static inline void DelManager()
{
	//_BaseManager->UnregisterManager(__uuidof(T));
}

template<class T>
static inline std::shared_ptr<T> GetManager()
{
	return std::dynamic_pointer_cast<T, IManager>(_BaseManager->GetManager(__uuidof(T)));
}