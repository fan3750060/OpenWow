#pragma once

#define SOCKETBASE_MAXBUFFERSIZE 4096

class CSocketBase : public ISocket
{
public:
	CSocketBase(cstring _host, cstring _port);
	~CSocketBase();

	void SendData(const IByteBufferOutput& _bb) override;
	void SendData(const IByteBuffer& _bb) override;
	void SendData(const uint8* _data, uint32 _count) override;

	void setOnReceiveCallback(Function_WA<ByteBuffer>* _onDataReceive);
	void callOnReceiveCallback(ByteBuffer _data);

	HANDLE& getEvent() { return m_SocketBaseThread; }
	SOCKET& getSocket() { return ConnectSocket; }
	uint32 getMyInet() const { return inet_addr("192.168.0.103"); }


private:
	SOCKET ConnectSocket;

	string m_Host;
	string m_Port;

	// Thread
	HANDLE m_SocketBaseThread;
	HANDLE m_Event_NeedExit;
	Function_WA<ByteBuffer>* m_OnDataReceive;
};