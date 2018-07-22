#include "stdafx.h"

// General
#include "ByteBuffer.h"

ByteBuffer::ByteBuffer() :
	m_IsFilled(false),
	m_IsEOF(true),
	m_IsAllocated(false),
	m_CurrentPosition(0)
{}

ByteBuffer::ByteBuffer(const ByteBuffer& _other) :
	m_IsFilled(false),
	m_IsEOF(true),
	m_IsAllocated(false),
	m_CurrentPosition(0)
{
	CopyData(_other.getData(), _other.getSize());

	m_IsEOF = _other.m_IsEOF;
	m_CurrentPosition = _other.m_CurrentPosition;
}

ByteBuffer::ByteBuffer(ByteBuffer&& _other)
{
	m_IsFilled = _other.m_IsFilled;
	m_IsEOF = _other.m_IsEOF;
	m_IsAllocated = _other.m_IsAllocated;
	m_Data = std::move(_other.m_Data);
	m_CurrentPosition = _other.m_CurrentPosition;
}

ByteBuffer::ByteBuffer(uint64 _size) :
	m_IsFilled(false),
	m_IsEOF(true),
	m_IsAllocated(false),
	m_CurrentPosition(0)
{
	Allocate(_size);
}

ByteBuffer::~ByteBuffer()
{
	if (!m_IsAllocated)
	{
		return;
	}
}

//--

ByteBuffer& ByteBuffer::operator=(const ByteBuffer& _other)
{
	m_IsFilled = false;
	m_IsEOF = true;
	m_IsAllocated = false;
	m_Data.clear();
	m_CurrentPosition = 0;

	CopyData(_other.getData(), _other.getSize());

	m_IsEOF = _other.m_IsEOF;
	m_CurrentPosition = _other.m_CurrentPosition;

	return *this;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& _other)
{
	m_IsFilled = _other.m_IsFilled;
	m_IsEOF = _other.m_IsEOF;
	m_IsAllocated = _other.m_IsAllocated;
	m_Data = std::move(_other.m_Data);
	m_CurrentPosition = _other.m_CurrentPosition;
	return *this;
}

//--

void ByteBuffer::Allocate(uint64 _size)
{
	m_IsEOF = true;

	if (_size > 0)
	{
		assert1(!m_IsAllocated);
		m_Data.reserve(_size);
		m_IsAllocated = true;
	}

	if (m_Data.size() < _size)
	{
		m_Data.resize(_size);
	}

	m_CurrentPosition = 0;
}

void ByteBuffer::SetFilled()
{
	m_IsEOF = (getSize() == 0);
	m_IsFilled = true;
}

void ByteBuffer::CopyData(const uint8* _data, uint64 _size)
{
	std::lock_guard<std::mutex> lg(m_Lock);

	if (!m_IsAllocated)
	{
		Allocate(_size);
	}

	if (_size > getSize())
	{
		Log::Error("ByteBuffer[]: Source m_Data size [%d] bigger than m_IsAllocated memory [%d]!", _size, getSize());
		Log::Error("ByteBuffer[]: Copy part of source m_Data.");
		_size = getSize();
	}

	if (_data != nullptr)
	{
		std::memcpy(&m_Data[0], _data, _size);
		SetFilled();
	}
}

//--

void ByteBuffer::seek(uint64 _bufferOffsetAbsolute)
{
	assert1(_bufferOffsetAbsolute <= getSize());

	if (_bufferOffsetAbsolute >= getSize())
	{
		m_CurrentPosition = getSize();
		m_IsEOF = true;
	}
	else
	{
		m_CurrentPosition = _bufferOffsetAbsolute;
		m_IsEOF = false;
	}
}

void ByteBuffer::seekRelative(uint64 _bufferOffsetRelative)
{
	assert1(m_CurrentPosition + _bufferOffsetRelative <= getSize());

	if (m_CurrentPosition + _bufferOffsetRelative >= getSize())
	{
		m_CurrentPosition = getSize();
		m_IsEOF = true;
	}
	else
	{
		m_CurrentPosition += _bufferOffsetRelative;
		m_IsEOF = false;
	}
}

//-- READ

void ByteBuffer::readLine(string* _string)
{
	std::lock_guard<std::mutex> lg(m_Lock);

	assert1(_string != nullptr);

	if (m_IsEOF)
	{
		return;
	}

	// Find first incorrect symbol
	uint64 lineEndPos;
	for (lineEndPos = m_CurrentPosition; (lineEndPos < getSize()) && (m_Data[lineEndPos] != '\n' && m_Data[lineEndPos] != '\r'); lineEndPos++);

	// Find first correct symbol after incorrects
	uint64 nextLineBeginPos;
	for (nextLineBeginPos = lineEndPos; (nextLineBeginPos < getSize()) && (m_Data[nextLineBeginPos] == '\n' || m_Data[nextLineBeginPos] == '\r'); nextLineBeginPos++);

	uint64 charsCount = lineEndPos - m_CurrentPosition;

	char* buff = new char[charsCount + 1];
	buff[charsCount] = '\0';
	readBytes(&buff[0], charsCount);

	// Skip \r and \n
	seekRelative(nextLineBeginPos - lineEndPos);

	string line(buff);
	delete[] buff;
	line = Utils::Trim(line);

	(*_string) = line;
}

void ByteBuffer::readBytes(void* _destination, uint64 _size)
{
	std::lock_guard<std::mutex> lg(m_Lock);

	if (m_IsEOF)
	{
		return;
	}

	uint64 posAfterRead = m_CurrentPosition + _size;
	if (posAfterRead > getSize())
	{
		_size = getSize() - m_CurrentPosition;
		m_IsEOF = true;
	}

	std::memcpy(_destination, &(m_Data[m_CurrentPosition]), _size);

	m_CurrentPosition = posAfterRead;
}

void ByteBuffer::readString(string* _string)
{
	assert1(_string != nullptr);

	string str = "";
	while (true)
	{
		uint8 byte;
		readBytes(&byte);

		str.append(1, static_cast<char>(byte));

		if (byte == '\0')
		{
			break;
		}
	}

	(*_string) = str;
}

//-- WRITE

void ByteBuffer::Append(const uint8* _data, uint64 _size)
{
	std::lock_guard<std::mutex> lg(m_Lock);

	assert1(_data != nullptr);

	for (uint64 i = 0; i < _size; i++)
	{
		uint64 pos = m_CurrentPosition + i;
		m_Data.push_back(_data[i]);
	}

	if (_size != 0)
	{
		m_IsEOF = false;
	}
}

void ByteBuffer::Write(int8 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBuffer::Write(int16 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBuffer::Write(int32 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBuffer::Write(int64 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBuffer::Write(uint8 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBuffer::Write(uint16 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBuffer::Write(uint32 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBuffer::Write(uint64 _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBuffer::Write(float _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBuffer::Write(double _data)
{
	Append((uint8*)&_data, sizeof(_data));
}

void ByteBuffer::Write(IByteBuffer& _other)
{
	Append(_other.getData(), _other.getSize());
}

void ByteBuffer::Write(const uint8* _string, uint64 _size)
{
	Append(_string, _size);
}

void ByteBuffer::Write(cstring _string, uint64 _expectedSize)
{
	Append((uint8*)_string.c_str(), static_cast<uint64>(_string.size()));

	if ((_string.size() < _expectedSize) && (_expectedSize != UINT32_MAX))
	{
		WriteDummy(_expectedSize - _string.size());
	}
}

void ByteBuffer::WriteDummy(uint64 _size)
{
	for (uint64 i = 0; i < _size; i++)
	{
		Write((uint8)0x00);
	}
}