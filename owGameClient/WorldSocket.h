#pragma once

#include "AuthCrypt.h"
#include "Opcodes.h"
#include "InPacket.h"
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

	void SendData(Opcodes _opcode);
	void SendData(Opcodes _opcode, CByteBuffer& _bb);
	void SendData(const uint8* _data, uint32 _count);

    void OnConnect() override final;
    void OnRawData(const char *buf, size_t len) override final;

	// Handlers
	void InitHandlers();
	void OnDataReceive(CByteBuffer _buf);

	// Handlers
	void AddHandler(Opcodes _opcode, HandlerFuncitonType _func);
	void ProcessHandler(Opcodes _handler, CByteBuffer _buffer);

	// Build packet
	InPacket* currPacket;
	void Packet1(uint16 _command, uint32 _size);
	void Packet2(CByteBuffer& _buf);

	void S_AuthChallenge(CByteBuffer& _buff);
	void S_AuthResponse(CByteBuffer& _buff);

private:
    std::weak_ptr<CWoWClient>                           m_WoWClient;
	AuthCrypt                                           m_WoWCryptoUtils;

	std::unordered_map<Opcodes, HandlerFuncitonType>	m_Handlers;
};