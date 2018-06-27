#pragma once

__interface ISocket
{
	void SendData(const IByteBufferOutput& _bb);
	void SendData(const IByteBuffer& _bb);
	void SendData(const uint8* _data, uint32 _count);
};