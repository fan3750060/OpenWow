#pragma once

#include "AuthCodes.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")



/*#define DEFAULT_HOST "176.213.201.73"
#define DEFAULT_PORT "3724"
#define DEFAULT_USERNAME "bouzi"
#define DEFAULT_PASS "automat"*/

class CAuthSocket
{
public:
	CAuthSocket(const char* _host, const char* _port);
	~CAuthSocket();

	void SendData(const uint8* _data, uint32 _count);
	void ReceiveLoop();

	void SendLogonChallenge();
	void ProcessHandler(eAuthCmd handler);

	bool H_LoginChallenge(ByteBuffer& _buff);

	//--

	uint32 getMyInet() { return inet_addr("192.168.0.103"); }

private:
	SOCKET ConnectSocket;
	ByteBuffer m_ByteBuffer;

	std::unordered_map<eAuthCmd, bool (CAuthSocket::*)(ByteBuffer&)> m_Handlers;

	SHA1Hash MServer;
};