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
	BigNumber* getKey() { return &m_Key; }

private:
	vector<RealmInfo>	m_Realms;

	BigNumber			m_Key;
	CAuthSocket*		m_AuthSocket;
	CWorldSocket*		m_WorldSocket;
};