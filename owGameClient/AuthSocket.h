#pragma once

#include "AuthCodes.h"

#include "SHA1.h"
#include "BigNumber.h"

// FORWARD BEGIN
class CWoWClient;
// FORWARD END

class CAuthSocket : public TcpSocket
{
	typedef bool (CAuthSocket::* HandlerFunc)(CByteBuffer&);
public:
	CAuthSocket(ISocketHandler& SocketHandler, std::shared_ptr<CWoWClient> WoWClient);
	~CAuthSocket();

	void SendData(const IByteBuffer& _bb);
	void SendData(const uint8* _data, uint32 _count);

    virtual void OnConnect() override;
    virtual void OnRawData(const char *buf, size_t len) override;

	// Handlers
	void InitHandlers();
	void OnDataReceive(CByteBuffer& _buf);
	void ProcessHandler(eAuthCmd _handler, CByteBuffer& _buffer);

	// Client
	void C_SendLogonChallenge();

	// Server
	bool S_LoginChallenge(CByteBuffer& _buff);
	bool S_LoginProof(CByteBuffer& _buff);
	bool S_Realmlist(CByteBuffer& _buff);

private:
	std::weak_ptr<CWoWClient> m_WoWClient;

	std::unordered_map<eAuthCmd, HandlerFunc> m_Handlers;

	BigNumber Key;
	SHA1Hash  MServer;
};