#pragma once

class ByteBuffer : public IByteBuffer
{
public:
	ByteBuffer();
	ByteBuffer(const ByteBuffer& _other);
	ByteBuffer(ByteBuffer&&) = delete;
	ByteBuffer(uint64_t _size);
	ByteBuffer(uint8* _data, uint64_t _size);
	~ByteBuffer();

	ByteBuffer& operator=(const ByteBuffer& _other);
	ByteBuffer& operator=(ByteBuffer&&) = delete;

	void Allocate(uint64_t _size);
	void SetFilled();
	void CopyData(const uint8* _data, uint64_t _size);
	void Init(uint8* _dataPtr, uint64_t _size);
    void Clear();
	inline uint8* GetAccessToData() { return m_Data; }

	// IByteBuffer
	uint64_t getSize() const override { return m_BufferSize; }
	uint64_t getPos() const override { return m_CurrentPosition; }
	const uint8* getData() const override { return m_Data; }
	const uint8* getDataFromCurrent() const override { return m_Data + m_CurrentPosition; }
	bool isEof() const override { return m_IsEOF; }
	
	// IByteBufferEx
	uint8* getDataEx() const { return &m_Data[0]; }
	uint8* getDataFromCurrentEx() const { return &m_Data[m_CurrentPosition]; }

	void seek(uint64_t _bufferOffsetAbsolute) override;
	void seekRelative(uint64_t _bufferOffsetRelative) override;
	string readLine() override;
	void readBytes(void* _destination, uint64_t _size = 1) override;
	void readString(string* _string);

private:
	bool		m_IsFilled;
	bool		m_IsOnlyPointer;

	bool		m_IsEOF;
	bool		m_IsAllocated;
	uint8*		m_Data;
	uint64_t	m_CurrentPosition;
	uint64_t	m_BufferSize;
};