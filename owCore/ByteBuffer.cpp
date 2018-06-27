#include "stdafx.h"

// General
#include "ByteBuffer.h"

ByteBuffer::ByteBuffer() :
	m_IsFilled(false),
	m_IsOnlyPointer(false),
	m_IsEOF(true),
	m_IsAllocated(false),
	m_Data(nullptr),
	m_CurrentPosition(0),
	m_BufferSize(0)
{}

ByteBuffer::ByteBuffer(const ByteBuffer& _other) :
	m_IsFilled(false),
	m_IsOnlyPointer(false),
	m_IsEOF(true),
	m_IsAllocated(false),
	m_Data(nullptr),
	m_CurrentPosition(0),
	m_BufferSize(0)
{
	CopyData(_other.getData(), _other.getSize());

	m_IsEOF = _other.m_IsEOF;
	m_CurrentPosition = _other.m_CurrentPosition;
}

ByteBuffer::ByteBuffer(uint64_t _size) :
	m_IsFilled(false),
	m_IsOnlyPointer(false),
	m_IsEOF(true),
	m_IsAllocated(false),
	m_Data(nullptr),
	m_CurrentPosition(0),
	m_BufferSize(0)
{
	Allocate(_size);
}

ByteBuffer::ByteBuffer(uint8* _data, uint64_t _size) :
	m_IsFilled(false),
	m_IsOnlyPointer(false),
	m_IsEOF(true),
	m_IsAllocated(false),
	m_Data(nullptr),
	m_CurrentPosition(0),
	m_BufferSize(0)
{

	Init(_data, _size);
}

ByteBuffer::~ByteBuffer()
{
	Clear();
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer & _other)
{
	m_IsFilled = false;
	m_IsOnlyPointer = false;
	m_IsEOF = true;
	m_IsAllocated = false;
	m_Data = nullptr;
	m_CurrentPosition = 0;
	m_BufferSize = 0;

	CopyData(_other.getData(), _other.getSize());

	m_IsEOF = _other.m_IsEOF;
	m_CurrentPosition = _other.m_CurrentPosition;

	return *this;
}

//

void ByteBuffer::Allocate(uint64_t _size)
{
	m_IsEOF = true;

	if (_size > 0)
	{
		assert1(!m_IsAllocated);
		m_Data = new uint8[_size];
		m_IsAllocated = true;
	}

	m_CurrentPosition = 0;
	m_BufferSize = _size;
}

void ByteBuffer::SetFilled()
{
	m_IsEOF = (m_BufferSize == 0);

	m_IsFilled = true;
	m_IsOnlyPointer = false;
}

void ByteBuffer::CopyData(const uint8* _data, uint64_t _size)
{
	if (!m_IsAllocated)
	{
		Allocate(_size);
	}

	if (_size > m_BufferSize)
	{
		Log::Error("ByteBuffer[]: Source m_Data size [%d] bigger than m_IsAllocated memory [%d]!", _size, m_BufferSize);
		Log::Error("ByteBuffer[]: Copy part of source m_Data.");
		_size = m_BufferSize;
	}

	if (_data != nullptr)
	{
		memcpy(m_Data, _data, _size);
		SetFilled();
	}
}

void ByteBuffer::Init(uint8* _dataPtr, uint64_t _size)
{
	m_IsEOF = (_size == 0);

	if (_size > 0)
	{
		if (_dataPtr != nullptr)
		{
			m_Data = _dataPtr;
			m_IsFilled = true;
			m_IsOnlyPointer = true;
		}
	}

	m_CurrentPosition = 0;
	m_BufferSize = _size;
}

void ByteBuffer::Clear()
{
	m_IsFilled = false;
	m_IsOnlyPointer = false;
	m_IsEOF = true;
	m_IsAllocated = false;
	m_CurrentPosition = 0;
	m_BufferSize = 0;

	if (!m_IsAllocated)
	{
		return;
	}

	if (m_IsOnlyPointer)
	{
		return;
	}

	if (m_Data != nullptr)
	{
		//Log::Error("Data deleted!!!");
		delete[] m_Data;
		m_Data = nullptr;
	}
}

//

void ByteBuffer::seek(uint64_t _bufferOffsetAbsolute)
{
	if (_bufferOffsetAbsolute >= m_BufferSize)
	{
		m_CurrentPosition = m_BufferSize;
		m_IsEOF = true;
	}
	else
	{
		m_CurrentPosition = _bufferOffsetAbsolute;
		m_IsEOF = false;
	}
}

void ByteBuffer::seekRelative(uint64_t _bufferOffsetRelative)
{
	if (m_CurrentPosition + _bufferOffsetRelative >= m_BufferSize)
	{
		m_CurrentPosition = m_BufferSize;
		m_IsEOF = true;
	}
	else
	{
		m_CurrentPosition += _bufferOffsetRelative;
		m_IsEOF = false;
	}
}

string ByteBuffer::readLine()
{
	if (m_IsEOF)
	{
		return "";
	}

	// Find first incorrect symbol
	uint64_t lineEndPos;
	for (lineEndPos = m_CurrentPosition; (lineEndPos < m_BufferSize) && (m_Data[lineEndPos] != '\n' && m_Data[lineEndPos] != '\r'); lineEndPos++);

	// Find first correct symbol after incorrects
	uint64_t nextLineBeginPos;
	for (nextLineBeginPos = lineEndPos; (nextLineBeginPos < m_BufferSize) && (m_Data[nextLineBeginPos] == '\n' || m_Data[nextLineBeginPos] == '\r'); nextLineBeginPos++);

	uint64_t charsCount = lineEndPos - m_CurrentPosition;

	char* _string = new char[charsCount + 1];
	_string[charsCount] = '\0';
	readBytes(&_string[0], charsCount);

	// Skip \r and \n
	seekRelative(nextLineBeginPos - lineEndPos);

	string line(_string);

	delete[] _string;

	return Utils::Trim(line);
}

void ByteBuffer::readBytes(void* _destination, uint64_t _size)
{
	if (m_IsEOF)
	{
		fail1();
		return;
	}

	uint64_t posAfterRead = m_CurrentPosition + _size;
	if (posAfterRead > m_BufferSize)
	{
		_size = m_BufferSize - m_CurrentPosition;
		m_IsEOF = true;
	}

	memcpy(_destination, &(m_Data[m_CurrentPosition]), _size);

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

		if (byte == '\0')
		{
			break;
		}

		str += (char)byte;
	}

	(*_string) = str;
}
