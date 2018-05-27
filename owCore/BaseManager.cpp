#include "stdafx.h"

// General
#include "BaseManager.h"

void CBaseManager::RegisterManager(Managers _type, IManager* _manager)
{
	m_Managers.insert(make_pair(_type, _manager));
}

void CBaseManager::UnregisterManager(IManager* _manager)
{
	//m_Managers.erase(std::remove(m_Managers.begin(), m_Managers.end(), _manager), m_Managers.end());
}

IManager* CBaseManager::GetManager(Managers _type)
{
	return m_Managers.at(_type);
}
