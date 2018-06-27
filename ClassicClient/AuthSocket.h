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
	CAuthSocket(CAuthWorldController* _world, cstring _host, cstring _port);
	~CAuthSocket();

	void SendData(const IByteBufferOutput& _bb) override;
	void SendData(const IByteBuffer& _bb) override;
	void SendData(const uint8* _data, uint32 _count) override;

	// Handlers
	void InitHandlers();
	void OnDataReceive(ByteBuffer _buf);
	void ProcessHandler(eAuthCmd _handler, ByteBuffer _buffer);

	// Client
	void C_SendLogonChallenge(string _username, string _password);

	// Server
	bool S_LoginChallenge(ByteBuffer& _buff);
	bool S_LoginProof(ByteBuffer& _buff);
	bool S_Realmlist(ByteBuffer& _buff);

private:
	CAuthWorldController* m_World;
	CSocketBase* socketBase;

	std::unordered_map<eAuthCmd, HandlerFunc> m_Handlers;

	string m_Host;
	string m_Port;
	string m_Username;
	string m_Password;

	BigNumber Key;
	SHA1Hash MServer;
};