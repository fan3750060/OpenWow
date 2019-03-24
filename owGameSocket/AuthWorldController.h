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
	const std::string& getHost() const { return m_Host; }
	const std::string& getPort() const { return m_Port; }
	const std::string& getUsername() const { return m_Username; }
	const std::string& getPassword() const { return m_Password; }
	BigNumber* getKey() { return &m_Key; }

	CAuthSocket* getAuthSocket() const { return m_AuthSocket; }
	CWorldSocket* getWorldSocket() const { return m_WorldSocket; }

private:
	std::string				m_Host;
	std::string				m_Port;
	std::string				m_Username;
	std::string				m_Password;
	BigNumber			m_Key;

	std::vector<RealmInfo>	m_Realms;

	CAuthSocket*		m_AuthSocket;
	CWorldSocket*		m_WorldSocket;
};