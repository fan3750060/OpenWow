#include "stdafx.h"

// General
#include "ByteBufferOutput.h"

void ByteBufferOutput::Write(int8 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBufferOutput::Write(int16 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBufferOutput::Write(int32 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBufferOutput::Write(int64 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBufferOutput::Write(uint8 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBufferOutput::Write(uint16 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBufferOutput::Write(uint32 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBufferOutput::Write(uint64 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBufferOutput::Write(ByteBufferOutput & _other)
{
	Append(_other.getData(), _other.getSize());
}

void ByteBufferOutput::Write(const uint8* _string, uint32 _size)
{
	Append(_string, _size);
}

void ByteBufferOutput::Write(cstring _string, uint32 _expectedSize)
{
	Append((uint8*)_string.c_str(), static_cast<uint32>(_string.size()));
	if ((_string.size() < _expectedSize) && (_expectedSize != UINT32_MAX))
	{
		for (uint32 i = 0; i < _expectedSize - _string.size(); i++)
		{
			Write((uint8)0x00);
		}
	}
}

void ByteBufferOutput::WriteDummy(uint32 _size)
{
	for (uint32 i = 0; i < _size; i++)
	{
		Write((uint8)0x00);
	}
}

//--

void ByteBufferOutput::Append(const uint8* _data, uint32 size)
{
	for (uint32 i = 0; i < size; i++)
	{
		m_Data.push_back(_data[i]);
	}
}
