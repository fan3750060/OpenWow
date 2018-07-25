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
	CWorldSocket(CAuthWorldController* _world);
	~CWorldSocket();

	void SendData(Opcodes _opcode);
	void SendData(Opcodes _opcode, ByteBuffer& _bb);
	void SendData(const uint8* _data, uint32 _count);

	// Thread
	void WorldThread(std::future<void> futureObj);

	// Handlers
	void Create(RealmInfo* _realm);
	void InitHandlers();
	void OnDataReceive(ByteBuffer _buf);

	// Handlers
	void AddHandler(Opcodes _opcode, Function_WA<ByteBuffer&>* _func);
	void ProcessHandler(Opcodes _handler, ByteBuffer _buffer);

	// Build packet
	InPacket* currPacket;
	void Packet1(uint16 _command, uint32 _size);
	void Packet2(ByteBuffer& _buf);

	void S_AuthChallenge(ByteBuffer& _buff);
	void S_AuthResponse(ByteBuffer& _buff);

	//--
	BigNumber getKey() const { return Key; }

private:
	CAuthWorldController*						m_World;
	CSocketBase*								socketBase;
	AuthCrypt									cryptUtils;

	// Thread
	std::promise<void>							m_ThreadPromise;
	std::thread									m_Thread;

	std::unordered_map<Opcodes, Function_WA<ByteBuffer&>*>	m_Handlers;

	RealmInfo*									m_Realm;

	BigNumber									Key;
};