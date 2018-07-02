#pragma once

#include "AuthCrypt.h"
#include "Opcodes.h"
#include "InPacket.h"
#include "RealmInfo.h"

// FORWARD BEGIN
class CAuthWorldController;
// FORWARD END

class CWorldSocket
{
	typedef bool (CWorldSocket::* HandlerFunc)(ByteBuffer&);
public:
	CWorldSocket(CAuthWorldController* _world, RealmInfo* _realm);
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

	bool S_CharEnum(ByteBuffer& _buff);

	//--
	BigNumber getKey() const { return Key; }


private:
	CAuthWorldController*	m_World;
	CSocketBase*			socketBase;
	AuthCrypt				cryptUtils;

	std::unordered_map<Opcodes, HandlerFunc> m_Handlers;
	std::queue<InPacket> m_Packets;

	RealmInfo*	m_Realm;

	BigNumber Key;
};