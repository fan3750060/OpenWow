#include "stdafx.h"

// General
#include "SocketBase.h"

CSocketBase::CSocketBase(const std::string& _host, const std::string& _port) :
	m_SocketObj(INVALID_SOCKET),
	m_Host(_host),
	m_Port(_port),
	m_OnDataReceive(nullptr)
{

	// Initialize Winsock
	Log::Info("Winsock: Start initializing...");

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		Log::Error("Winsock: Init failed with error: %d", iResult);
		fail1();
	}
	
	Log::Info("Winsock: Inited!");
	Log::Info("Winsock: Description [%s]", wsaData.szDescription);
	Log::Info("Winsock: Status [%s]", wsaData.szSystemStatus);

	Start();
}

CSocketBase::~CSocketBase()
{
	Stop();

	WSACleanup();

	Log::Info("[SocketBase]: Deleted.");
}

void CSocketBase::Start()
{
	ADDRINFOA hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	PADDRINFOA addrInfo = NULL;
	int iResult = GetAddrInfoA(m_Host.c_str(), m_Port.c_str(), &hints, &addrInfo);
	if (iResult != 0)
	{
		Log::Error("[SocketBaseThread]: 'getaddrinfo' failed with error: %d", iResult);
		fail1();
	}

	// Attempt to connect to an address until one succeeds
	for (PADDRINFOA ptr = addrInfo; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		m_SocketObj = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (m_SocketObj == INVALID_SOCKET)
		{
			Log::Error("[SocketBaseThread]: 'socket' failed with error: %ld", WSAGetLastError());
			fail1();
		}

		// Connect to server
		iResult = connect(m_SocketObj, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(m_SocketObj);
			m_SocketObj = INVALID_SOCKET;
			continue;
		}

		break;
	}

	FreeAddrInfoA(addrInfo);

	if (m_SocketObj == INVALID_SOCKET)
	{
		Log::Error("[SocketBaseThread]: Unable to connect to server!");
		fail1();
	}

	// Nonblocking socket
	u_long iMode = 1;
	ioctlsocket(m_SocketObj, FIONBIO, &iMode);

	m_ReadCache = new SocketCache(PacketType::READ);
	m_WriteCache = new SocketCache(PacketType::WRITE);

	// Threads
	std::future<void> futureObjRead = m_ReadThreadPromise.get_future();
	m_ReadThread = std::thread(&CSocketBase::SocketReadThread, this, std::move(futureObjRead));
	m_ReadThread.detach();

	std::future<void> futureObjWrite = m_WriteThreadPromise.get_future();
	m_WriteThread = std::thread(&CSocketBase::SocketWriteThread, this, std::move(futureObjWrite));
	m_WriteThread.detach();
}

void CSocketBase::Stop()
{
	m_ReadThreadPromise.set_value();
	if (m_ReadThread.joinable()) m_ReadThread.join();

	m_WriteThreadPromise.set_value();
	if (m_WriteThread.joinable()) m_WriteThread.join();

	Log::Info("[SocketBase]: All threads stopped.");

	// shutdown the connection since no more data will be sent
	int result = shutdown(m_SocketObj, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		Log::Error("[CSocketBase]: shutdown failed with error: %d", WSAGetLastError());
		closesocket(m_SocketObj);
		fail1();
	}

	// cleanup
	closesocket(m_SocketObj);
}

//--

void CSocketBase::SendData(const IByteBuffer& _bb)
{
	SendData(_bb.getData(), _bb.getSize());
}
void CSocketBase::SendData(const uint8* _data, uint32 _count)
{
	getWriteCache()->Add(_data, _count);
}

void CSocketBase::SocketReadThread(std::future<void> _future)
{
	fd_set fdRead;

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	while (_future.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		FD_ZERO(&fdRead);
		FD_SET(m_SocketObj, &fdRead);

		m_SocketObjBusy.lock();
		int selectResult = select(0, &fdRead, NULL, NULL, &tv);
		m_SocketObjBusy.unlock();
		if (selectResult < 0)
		{
			int32 lastError = WSAGetLastError();
			Log::Error("[SocketReadThread]: 'select' failed with error: [%d][0x%X].", lastError, lastError);
			continue;
		}

		if (selectResult != 0 && FD_ISSET(m_SocketObj, &fdRead))
		{
			char receiveBuffer[SOCKETBASE_MAXBUFFERSIZE];
			int32 receiveBufferSize = SOCKETBASE_MAXBUFFERSIZE;
			m_SocketObjBusy.lock();
			int32 currentSize = recv(m_SocketObj, receiveBuffer, receiveBufferSize, 0);
			m_SocketObjBusy.unlock();
			assert1(currentSize < SOCKETBASE_MAXBUFFERSIZE);

			if (currentSize > 0)
			{
				// Cache
				getReadCache()->Add((uint8*)receiveBuffer, currentSize);

				// Event
				if (m_OnDataReceive != nullptr)
				{
					callOnReceiveCallback((uint8*)receiveBuffer, currentSize);
				}

				//Log::Info("[SocketReadThread]: 'recv' size=%d", currentSize);
			}
			else if (currentSize == 0)
			{
				Log::Error("[SocketReadThread]: Connection closed.");
				break;
			}
			else
			{
				int32 lastError = WSAGetLastError();
				Log::Error("[SocketReadThread]: 'recv' failed with error: '%d' '0x%X'.", lastError, lastError);
			}
		}
	};

	Log::Info("[SocketReadThread]: Exit thread.");
}

void CSocketBase::SocketWriteThread(std::future<void> _future)
{
	while (_future.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		if (getWriteCache()->isReady())
		{
			CByteBuffer buff = std::move(getWriteCache()->Pop());

			m_SocketObjBusy.lock();
			int32 result = send(m_SocketObj, (const char*)buff.getData(), buff.getSize(), 0);
			m_SocketObjBusy.unlock();
			if (result == SOCKET_ERROR)
			{
				int32 lastError = WSAGetLastError();
				Log::Error("[SocketWriteThread]: 'send' failed with error: '%d' '0x%X'.", lastError, lastError);
			}

			//Log::Info("[socket->send] [%d] bytes.", result);
		}
	}

	Log::Info("[SocketWriteThread]: Exit thread.");
}

// Callback

void CSocketBase::setOnReceiveCallback(Function_WA<CByteBuffer>* _onDataReceive)
{
	SafeDelete(m_OnDataReceive);
	m_OnDataReceive = _onDataReceive;
}
bool CSocketBase::isReceiveCallbackCorrect()
{
	return m_OnDataReceive != nullptr;
}
void CSocketBase::callOnReceiveCallback(uint8* _data, uint32 _size)
{
	assert1(m_OnDataReceive != nullptr);

	CByteBuffer byteBuffer;
	byteBuffer.CopyData(_data, _size);

	FUNCTION_WA_CALL(m_OnDataReceive, CByteBuffer, byteBuffer);
}

//-------

SocketCache::SocketCache(PacketType::List _cacheType) :
	m_CacheType(_cacheType),
	m_Counter(0)
{}

void SocketCache::Add(const uint8 * _data, uint32 _size)
{
	std::lock_guard<std::mutex> lg(m_CacheLock);

	Packet packet(m_Counter++, m_CacheType, _data, _size);
	m_Cache.push(std::move(packet));
}

bool SocketCache::isReady()
{
	std::lock_guard<std::mutex> lg(m_CacheLock);

	return !m_Cache.empty();
}

CByteBuffer SocketCache::Pop()
{
	std::lock_guard<std::mutex> lg(m_CacheLock);

	assert1(!m_Cache.empty());

	Packet& bb = m_Cache.front();
	CByteBuffer buf =  bb.getData();
	m_Cache.pop();

	return std::move(buf);
}

Packet::Packet(uint64 _id, PacketType::List _type, CByteBuffer& _buffer) :
	m_Id(_id),
	m_PacketType(_type)
{
	m_Buffer = std::move(_buffer);

	//Log::Info("%s: created, size='%d'", getName().c_str(), m_Buffer.getSize());
}

Packet::Packet(uint64 _id, PacketType::List _type, const uint8* _data, uint32 _size) :
	m_Id(_id),
	m_PacketType(_type)
{
	m_Buffer.CopyData(_data, _size);

	//Log::Info("%s: created, size='%d'", getName().c_str(), m_Buffer.getSize());
}

Packet::Packet(Packet&& _other)
{
	m_Id = _other.m_Id;
	m_PacketType = _other.m_PacketType;
	m_Buffer = std::move(_other.getData());

	//Log::Info("%s: moved", getName().c_str(), m_Id);
}

Packet::~Packet()
{
	//Log::Info("%s: destroyed", getName().c_str());
}

Packet& Packet::operator=(Packet && _other)
{
	m_Id = _other.m_Id;
	m_PacketType = _other.m_PacketType;
	m_Buffer = std::move(_other.getData());

	return *this;
}
