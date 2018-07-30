#pragma once

#include <mutex>

static_assert(sizeof(size_t) == sizeof(uint64), "Size of 'size_t' isn't equal 'uint64_t'");

class ByteBuffer : public IByteBuffer
{
public:
	ByteBuffer();
	ByteBuffer(const ByteBuffer& _other);
	ByteBuffer(ByteBuffer&& _other);
	ByteBuffer(uint64 _size);
	~ByteBuffer();

	ByteBuffer& operator=(const ByteBuffer& _other);
	ByteBuffer& operator=(ByteBuffer&&  _other);

	void Allocate(uint64 _size);
	void SetFilled();
	void CopyData(const uint8* _data, uint64 _size);

	// IByteBuffer
	uint64 getSize() const override { return m_Data.size(); }
	uint64 getPos() const override { return m_CurrentPosition; }
	const uint8* getData() const override { return &m_Data[0]; }
	const uint8* getDataFromCurrent() const override { return &m_Data[m_CurrentPosition]; }
	bool isEof() const override { return m_IsEOF; }

	// IByteBufferEx
	uint8* getDataEx() { return &m_Data[0]; }
	uint8* getDataFromCurrentEx() { return &m_Data[m_CurrentPosition]; }

	void seek(uint64 _bufferOffsetAbsolute) override;
	void seekRelative(uint64 _bufferOffsetRelative) override;

	// Read
	void readLine(string* _string) override;
	void readBytes(void* _destination, uint64 _size = 1) override;
	void readString(string* _string) override;

	//-- SIGNED
	ByteBuffer& operator>>(int8 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator>>(int16 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator>>(int32 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator>>(int64 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	//-- UNSIGNED
	ByteBuffer& operator>>(uint8 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator>>(uint16 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator>>(uint32 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator>>(uint64 &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	//-- FLOAT
	ByteBuffer& operator>>(float &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator>>(double &value)
	{
		readBytes(&value, sizeof(value));
		return *this;
	}
	//-- STRING
	ByteBuffer& operator>>(string& _string)
	{
		_string = "";

		while (true)
		{
			uint8 byte;
			readBytes(&byte);

			_string.append(1, static_cast<char>(byte));

			if (byte == '\0')
			{
				break;
			}
		}
	}

	// Write
	void Append(const uint8* _data, uint64 _size);

	void Write(cstring _string, uint64 _expectedSize = UINT32_MAX);
	void WriteDummy(uint64 _size);

	ByteBuffer& operator<<(int8 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator<<(int16 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator<<(int32 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator<<(int64 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	//----
	ByteBuffer& operator<<(uint8 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator<<(uint16 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator<<(uint32 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator<<(uint64 value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	//--
	ByteBuffer& operator<<(float value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	ByteBuffer& operator<<(double value)
	{
		Append((uint8*)&value, sizeof(value));
		return *this;
	}
	//--
	ByteBuffer& operator<<(ByteBuffer& _other)
	{
		Append(_other.getData(), _other.getSize());
		return *this;
	}
	ByteBuffer& operator<<(const char* _string)
	{
		size_t len = strlen(_string);
		if (len > 0)
		{
			Append((const uint8*)_string, len);
		}
		Append((uint8)0x00, 1);
		return *this;
	}
	ByteBuffer& operator<<(cstring _string)
	{
		Append((uint8*)_string.c_str(), static_cast<uint64>(_string.size()) + 1);
		return *this;
	}

private:
	bool            m_IsFilled;

	bool            m_IsEOF;
	bool		    m_IsAllocated;
	vector<uint8>   m_Data;
	uint64_t	    m_CurrentPosition;
};