#pragma once

#include <chrono>
#include <future>

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

#define SOCKETBASE_MAXBUFFERSIZE 200000//4096

struct PacketType
{
	enum List : uint32
	{
		READ = 0,
		WRITE
	};
};

class Packet
{
public:
	Packet(uint64 _id, PacketType::List _type, CByteBuffer& _buffer);
	Packet(uint64 _id, PacketType::List _type, const uint8* _data, uint32 _size);
	Packet(const Packet& _other) = delete;
	Packet(Packet&& _other);
	~Packet();

	Packet& operator=(const Packet&) = delete;
	Packet& operator=(Packet&& _other);

	CByteBuffer& getData() { return m_Buffer; }

private:
	std::string getName() { return std::string(m_PacketType == PacketType::READ ? "PacketRead" : "PacketWrite") + "[" + std::to_string(m_Id) + "]"; }

private:
	uint64				m_Id;
	PacketType::List	m_PacketType;
	CByteBuffer			m_Buffer;
};

//******

class SocketCache
{
public:
	SocketCache(PacketType::List _cacheType);

	// Read cache
	void Add(const uint8* _data, uint32 _size);
	bool isReady();
	CByteBuffer Pop();

private:
	std::mutex					m_CacheLock;
	std::queue<Packet>			m_Cache;
	PacketType::List			m_CacheType;
	uint64						m_Counter;
};


//******

class CSocketBase : public ISocket
{
public:
	CSocketBase(const std::string& _host, const std::string& _port);
	~CSocketBase();

	void Start();
	void Stop();

	void SendData(const IByteBuffer& _bb) override;
	void SendData(const uint8* _data, uint32 _count) override;

	// Thread
	void SocketReadThread(std::future<void> _future);
	void SocketWriteThread(std::future<void>  _future);

	SocketCache* getReadCache() { return m_ReadCache; }
	SocketCache* getWriteCache() { return m_WriteCache; }

	// Receive callback
	void setOnReceiveCallback(Function_WA<CByteBuffer>* _onDataReceive);
	bool isReceiveCallbackCorrect();
	void callOnReceiveCallback(uint8* _data, uint32 _size);

	uint32 getMyInet() const { return inet_addr("192.168.0.103"); }

private:
	std::mutex					m_SocketObjBusy;
	SOCKET						m_SocketObj;

	std::string						m_Host;
	std::string						m_Port;

	// Thread
	std::promise<void>			m_ReadThreadPromise;
	std::thread					m_ReadThread;

	std::promise<void>			m_WriteThreadPromise;
	std::thread					m_WriteThread;

	// Event
	Function_WA<CByteBuffer>*	m_OnDataReceive;

	// Cahce
	SocketCache*				m_ReadCache;
	SocketCache*				m_WriteCache;
};