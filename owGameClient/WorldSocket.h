#pragma once

#include "AuthCrypt.h"
#include "Opcodes.h"
#include "ServerPacket.h"
#include "ClientPacket.h"
#include "RealmInfo.h"

// FORWARD BEGIN
class CWoWClient;
// FORWARD END


struct ServerPktHeader
{
    uint16 size;
    uint16 cmd;
};

struct ClientPktHeader
{
    uint16 size;
    uint32 cmd;
};


class CWorldSocket : public TcpSocket
{
	typedef bool (CWorldSocket::* HandlerFunc)(CByteBuffer&);

public:
    typedef std::function<void(CByteBuffer&)> HandlerFuncitonType;
public:
	CWorldSocket(ISocketHandler& SocketHandler, std::shared_ptr<CWoWClient> WoWClient);
	~CWorldSocket();


    
    // TcpSocket
    void OnDisconnect() override final;
    void OnConnect() override final;
    void OnRawData(const char *buf, size_t len) override final;

	// CWorldSocket
    void SendPacket(CClientPacket& Packet);
    void SendData(const uint8* _data, uint32 _count);

    // Packets contructor
    void Packet1(uint16 Size, Opcodes Opcode);
    void Packet2(CByteBuffer& _buf);

	void InitHandlers();
	void AddHandler(Opcodes _opcode, HandlerFuncitonType _func);
	void ProcessPacket(CServerPacket ServerPacket);

    // Handlers
	void S_AuthChallenge(CByteBuffer& _buff);
	void S_AuthResponse(CByteBuffer& _buff);

private:
    std::weak_ptr<CWoWClient>                           m_WoWClient;
	AuthCrypt                                           m_WoWCryptoUtils;

    CServerPacket*                                      currPacket;

	std::unordered_map<Opcodes, HandlerFuncitonType>	m_Handlers;
};