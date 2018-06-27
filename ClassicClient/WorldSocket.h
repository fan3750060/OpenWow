#pragma once

#include "AuthCrypt.h"
#include "Opcodes.h"
#include "InPacket.h"

// FORWARD BEGIN
class CAuthWorldController;
// FORWARD END

class CWorldSocket
{
	typedef bool (CWorldSocket::* HandlerFunc)(ByteBuffer&);
public:
	CWorldSocket(CAuthWorldController* _world, string _host, string _port, string _username);
	~CWorldSocket();

	void SendData(Opcodes _opcode);
	void SendData(Opcodes _opcode, ByteBufferOutput& _bb);
	void SendData(const uint8* _data, uint32 _count);

	// Handlers
	void InitHandlers();
	void OnDataReceive(ByteBuffer _buf);
	void ProcessHandler(Opcodes _handler, ByteBuffer _buffer);

	bool S_AuthChallenge(ByteBuffer& _buff);
	bool S_AuthResponse(ByteBuffer& _buff);

	//--
	BigNumber getKey() const { return Key; }


private:
	CAuthWorldController*	m_World;
	CSocketBase*			socketBase;
	AuthCrypt				cryptUtils;

	std::unordered_map<Opcodes, HandlerFunc> m_Handlers;
	std::queue<InPacket> m_Packets;

	string m_Host;
	string m_Port;
	string m_Username;

	BigNumber Key;
};