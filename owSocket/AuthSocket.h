#pragma once

#include "SocketBase.h"
#include "AuthCodes.h"

// FORWARD BEGIN
class CAuthWorldController;
// FORWARD END

class CAuthSocket : public ISocket
{
	typedef bool (CAuthSocket::* HandlerFunc)(ByteBuffer&);
public:
	CAuthSocket(CAuthWorldController* _world);
	~CAuthSocket();

	void Stop();

	void SendData(const IByteBuffer& _bb) override;
	void SendData(const uint8* _data, uint32 _count) override;

	// Thread
	void AuthThread(std::future<void> futureObj);
	
	// Handlers
	void InitHandlers();
	void OnDataReceive(ByteBuffer& _buf);
	void ProcessHandler(eAuthCmd _handler, ByteBuffer& _buffer);

	// Client
	void C_SendLogonChallenge();

	// Server
	bool S_LoginChallenge(ByteBuffer& _buff);
	bool S_LoginProof(ByteBuffer& _buff);
	bool S_Realmlist(ByteBuffer& _buff);

private:
	CAuthWorldController* m_World;
	CSocketBase* socketBase;

	// Thread
	std::promise<void>			m_ThreadPromise;
	std::thread					m_Thread;

	std::unordered_map<eAuthCmd, HandlerFunc> m_Handlers;

	BigNumber Key;
	SHA1Hash MServer;
};