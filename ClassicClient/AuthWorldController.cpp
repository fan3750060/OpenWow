#include "stdafx.h"

// General
#include "AuthWorldController.h"

// My server
/*#define DEFAULT_HOST "176.213.201.73"
#define DEFAULT_PORT "3724"
#define DEFAULT_USERNAME "bouzi"
#define DEFAULT_PASS "automat"*/

// Logon3
#define DEFAULT_HOST "logon3.wowcircle.com"
#define DEFAULT_PORT "3724"


CAuthWorldController::CAuthWorldController()
{
	m_AuthSocket = new CAuthSocket(this, DEFAULT_HOST, DEFAULT_PORT);
}

void CAuthWorldController::StartAuth()
{
	m_AuthSocket->InitHandlers();
	m_AuthSocket->C_SendLogonChallenge(DEFAULT_USERNAME, DEFAULT_PASS);
}

void CAuthWorldController::OnSuccessConnect(BigNumber Key)
{
	m_Key = Key;
	delete m_AuthSocket;

	Sleep(500); // TODO

	RealmInfo* currRealm = &(m_Realms[0]);
	m_WorldSocket = new CWorldSocket(this, currRealm->getIP(), currRealm->getPort(), DEFAULT_USERNAME);
	m_WorldSocket->InitHandlers();
}

void CAuthWorldController::AddRealm(RealmInfo& _realm)
{
	m_Realms.push_back(_realm);
	_realm.print();
}
