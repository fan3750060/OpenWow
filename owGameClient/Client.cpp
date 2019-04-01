#include "stdafx.h"

// General
#include "Client.h"

CWoWClient::CWoWClient()
{
	m_Host = "192.168.1.50";
	m_Port = 3724;
	m_Username = "admin";
	m_Password = "admin";

    m_SocketsHandler = std::make_shared<SocketHandler>();
    m_SocketsHandler->RegStdLog(&m_SocketLog);  	
}

void CWoWClient::StartAuth()
{
    m_AuthSocket = std::make_shared<CAuthSocket>(*m_SocketsHandler, shared_from_this());
    m_AuthSocket->Open(getHost(), getPort());

    m_SocketsHandler->Add(m_AuthSocket.operator->());
    m_SocketsHandler->Select(1, 0);

    while (m_SocketsHandler->GetCount())
    {
        m_SocketsHandler->Select(1, 0);
    }
}

void CWoWClient::OnSuccessConnect(BigNumber Key)
{
	m_Key = Key;

    m_AuthSocket->SetErasedByHandler();

    RealmInfo* currRealm = &(m_Realms[0]);


    m_WorldSocket = std::make_shared<CWorldSocket>(*m_SocketsHandler, shared_from_this());
    m_WorldSocket->Open(currRealm->getIP(), currRealm->getPort());

    m_SocketsHandler->Add(m_WorldSocket.operator->());
}

void CWoWClient::AddRealm(RealmInfo& _realm)
{
	m_Realms.push_back(_realm);
	_realm.print();
}
