#include "stdafx.h"

// General
#include "AuthWorldController.h"


CAuthWorldController::CAuthWorldController()
{
	m_Host = "logon3.wowcircle.com";
	m_Port = "3724";
	m_Username = "openwowTest";
	m_Password = "123456789";

	m_AuthSocket = new CAuthSocket(this);
	m_WorldSocket = new CWorldSocket(this);
}

void CAuthWorldController::StartAuth()
{
	m_AuthSocket->InitHandlers();
	m_AuthSocket->C_SendLogonChallenge();
}

void CAuthWorldController::OnSuccessConnect(BigNumber Key)
{
	m_Key = Key;

	delete m_AuthSocket;

	RealmInfo* currRealm = &(m_Realms[0]);
	m_WorldSocket->Create(currRealm);
	m_WorldSocket->InitHandlers();
}

void CAuthWorldController::AddRealm(RealmInfo& _realm)
{
	m_Realms.push_back(_realm);
	_realm.print();
}
