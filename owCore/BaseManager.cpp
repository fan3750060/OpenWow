#include "stdafx.h"

// General
#include "BaseManager.h"

// Additional
#include "FilesManager.h"

std::shared_ptr<CBaseManager> _BaseManager;

CBaseManager::CBaseManager() :
	m_CurrentPhase(Phase_NONE)
{}

CBaseManager::~CBaseManager()
{
	OutputDebugString("CBaseManager destroyed.\n");
}

void CBaseManager::RegisterManager(GUID _type, std::shared_ptr<IManager> _manager)
{
	if (m_Managers.find(_type) != m_Managers.end())
	{
		fail1();
	}

	m_Managers.insert(std::make_pair(_type, _manager));
}

void CBaseManager::UnregisterManager(GUID _type)
{
	//m_Managers.erase(std::remove(m_Managers.begin(), m_Managers.end(), _type), m_Managers.end());
}

std::shared_ptr<IManager> CBaseManager::GetManager(GUID _type)
{
	if (m_Managers.find(_type) == m_Managers.end())
	{
		return nullptr;
	}

	return m_Managers.at(_type);
}

void CBaseManager::SetPhase(SBaseManagerPhases _phase)
{
	m_CurrentPhase = _phase;
}

SBaseManagerPhases CBaseManager::GetPhase()
{
	return m_CurrentPhase;
}
