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
	typedef bool (CWorldSocket::* HandlerFunc)(CByteBuffer&);
public:
	CWorldSocket(CAuthWorldController* _world);
	~CWorldSocket();

	void SendData(Opcodes _opcode);
	void SendData(Opcodes _opcode, CByteBuffer& _bb);
	void SendData(const uint8* _data, uint32 _count);

	// Thread
	void WorldThread(std::future<void> futureObj);

	// Handlers
	void Create(RealmInfo* _realm);
	void InitHandlers();
	void OnDataReceive(CByteBuffer _buf);

	// Handlers
	void AddHandler(Opcodes _opcode, Function_WA<CByteBuffer&>* _func);
	void ProcessHandler(Opcodes _handler, CByteBuffer _buffer);

	// Build packet
	InPacket* currPacket;
	void Packet1(uint16 _command, uint32 _size);
	void Packet2(CByteBuffer& _buf);

	void S_AuthChallenge(CByteBuffer& _buff);
	void S_AuthResponse(CByteBuffer& _buff);

	//--
	BigNumber getKey() const { return Key; }

private:
	CAuthWorldController*						m_World;
	CSocketBase*								socketBase;
	AuthCrypt									cryptUtils;

	// Thread
	std::promise<void>							m_ThreadPromise;
	std::thread									m_Thread;

	std::unordered_map<Opcodes, Function_WA<CByteBuffer&>*>	m_Handlers;

	RealmInfo*									m_Realm;

	BigNumber									Key;
};