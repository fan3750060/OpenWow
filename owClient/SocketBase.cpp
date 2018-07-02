#include "stdafx.h"

// General
#include "SocketBase.h"

DWORD WINAPI SocketBaseThread(LPVOID lpParam)
{
	CSocketBase* socket = (CSocketBase*)lpParam;
	do
	{
		char receiveBuffer[SOCKETBASE_MAXBUFFERSIZE];
		int32 receiveBufferSize = SOCKETBASE_MAXBUFFERSIZE;
		int32 currentSize = recv(socket->getSocket(), receiveBuffer, receiveBufferSize, 0);

		if (currentSize > 0)
		{
			ByteBuffer byteBuffer;
			byteBuffer.CopyData((uint8*)receiveBuffer, currentSize);
			Log::Info("[socket<-recv] [%d] bytes.", currentSize);
			socket->callOnReceiveCallback(byteBuffer);
			Sleep(25);
		}
		else if (currentSize == 0)
		{
			Log::Error("Connection closed");
			break;
		}
		else
		{
			int32 lastError = WSAGetLastError();
			/*if (lastError != WSAEWOULDBLOCK)
			{
				Log::Error("recv failed with error: [%d][0x%X]", WSAGetLastError(), WSAGetLastError());
				break;
			}*/
		}

	} while (true);

	Log::Info("CSocketBase Exit thread");
	ExitThread(0);
}

//--

CSocketBase::CSocketBase(cstring _host, cstring _port) :
	ConnectSocket(INVALID_SOCKET),
	m_Host(_host),
	m_Port(_port),
	m_OnDataReceive(nullptr)
{
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		Log::Error("WSAStartup failed with error: %d", iResult);
		fail1();
	}

	ADDRINFOA hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	PADDRINFOA addrInfo = NULL;
	iResult = GetAddrInfoA(m_Host.c_str(), m_Port.c_str(), &hints, &addrInfo);
	if (iResult != 0)
	{
		Log::Error("getaddrinfo failed with error: %d", iResult);
		WSACleanup();
		fail1();
	}

	// Attempt to connect to an address until one succeeds
	for (PADDRINFOA ptr = addrInfo; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET)
		{
			Log::Error("socket failed with error: %ld", WSAGetLastError());
			WSACleanup();
			fail1();
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}

		break;
	}

	FreeAddrInfoA(addrInfo);

	if (ConnectSocket == INVALID_SOCKET)
	{
		Log::Error("Unable to connect to server!");
		WSACleanup();
		fail1();
	}

	// Nonblocking socket
	//u_long iMode = 1;
	//ioctlsocket(ConnectSocket, FIONBIO, &iMode);

	// Event
	//m_Event_NeedExit = CreateEventW(NULL, TRUE, TRUE, NULL);

	// Thread
	m_SocketBaseThread = CreateThread(NULL, 0, &SocketBaseThread, this, NULL, NULL);
	SetThreadPriority(m_SocketBaseThread, THREAD_PRIORITY_NORMAL);
}

CSocketBase::~CSocketBase()
{
	//SetEvent(m_Event_NeedExit);
	CloseHandle(m_SocketBaseThread);

	// shutdown the connection since no more data will be sent
	int iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		Log::Error("shutdown failed with error: %d", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		fail1();
	}

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
}

//--

void CSocketBase::SendData(const IByteBufferOutput& _bb)
{
	SendData(_bb.getData(), _bb.getSize());
}

void CSocketBase::SendData(const IByteBuffer& _bb)
{
	SendData(_bb.getData(), _bb.getSize());
}

void CSocketBase::SendData(const uint8* _data, uint32 _count)
{
	int32 sendResult = send(ConnectSocket, (const char*)_data, _count, 0);
	if (sendResult == SOCKET_ERROR)
	{
		Log::Error("[socket->send] Failed with error [%X]", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		fail1();
	}

	Log::Info("[socket->send] [%d] bytes.", sendResult);
}
void CSocketBase::setOnReceiveCallback(Function_WA<ByteBuffer>* _onDataReceive)
{
	if (m_OnDataReceive != nullptr)
	{
		delete m_OnDataReceive;
	}

	m_OnDataReceive = _onDataReceive;
}

void CSocketBase::callOnReceiveCallback(ByteBuffer _data)
{
	assert1(m_OnDataReceive != nullptr);
	FUNCTION_WA_CALL(m_OnDataReceive, ByteBuffer, _data);
}
