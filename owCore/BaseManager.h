#pragma once

class CBaseManager : public IBaseManager
{
	CLASS_INSTANCE(CBaseManager)
public:
	void RegisterManager(Managers _type, IManager* _manager) override;
	void UnregisterManager(IManager* _manager) override;

	IManager* GetManager(Managers _type) override;

private:
	map<Managers, IManager*> m_Managers;
};

template<class T>
static inline T* GetManager(Managers _manager)
{
	return (T*)(CBaseManager::instance()->GetManager(_manager));
}

template<class T>
static inline T* GetManager()
{
	if (typeid(T) == typeid(ITexturesManager))
	{
		return (T*)(CBaseManager::instance()->GetManager(Managers::MgrTextures));
	}
	else if (typeid(T) == typeid(IWMOManager))
	{
		return (T*)(CBaseManager::instance()->GetManager(Managers::MgrWMO));
	}
	else if (typeid(T) == typeid(IMDXManager))
	{
		return (T*)(CBaseManager::instance()->GetManager(Managers::MgrMDX));
	}
	else if (typeid(T) == typeid(IFilesManager))
	{
		return (T*)(CBaseManager::instance()->GetManager(Managers::MgrFiles));
	}
}