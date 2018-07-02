#pragma once

#include "AuthSocket.h"
#include "WorldSocket.h"

#include "RealmInfo.h"

class CAuthWorldController
{
public:
	CAuthWorldController();

	void StartAuth();
	void OnSuccessConnect(BigNumber Key);

	void AddRealm(RealmInfo& _realm);

	// Getters
	cstring getHost() const { return m_Host; }
	cstring getPort() const { return m_Port; }
	cstring getUsername() const { return m_Username; }
	cstring getPassword() const { return m_Password; }
	BigNumber* getKey() { return &m_Key; }

private:
	string				m_Host;
	string				m_Port;
	string				m_Username;
	string				m_Password;
	BigNumber			m_Key;

	vector<RealmInfo>	m_Realms;

	CAuthSocket*		m_AuthSocket;
	CWorldSocket*		m_WorldSocket;
};