#pragma once

class ByteBuffer : public IByteBuffer
{
public:
	ByteBuffer();
	ByteBuffer(const ByteBuffer& _other);
	ByteBuffer(uint64_t _size);
	ByteBuffer(uint8* _data, uint64_t _size);
	~ByteBuffer();

	void Allocate(uint64_t _size);
	void SetFilled();
	void CopyData(uint8* _data, uint64_t _size);
	void Init(uint8* _dataPtr, uint64_t _size);
    void Clear();
	inline uint8* GetAccessToData() { return m_Data; }

	// IByteBuffer
	inline uint64_t GetSize() const override { return m_BufferSize; }
	inline uint64_t GetPos() const override { return m_CurrentPosition; }
	inline const uint8* GetData() const override { return m_Data; }
	inline const uint8* GetDataFromCurrent() const override { return m_Data + m_CurrentPosition; }
	inline bool IsEof() const override { return m_IsEOF; }
	
	void Seek(uint64_t _bufferOffsetAbsolute) override;
	void SeekRelative(uint64_t _bufferOffsetRelative) override;
	string ReadLine() override;
	void ReadBytes(void* _destination, uint64_t _size) override;

private:
	bool m_IsFilled;
	bool m_IsOnlyPointer;

	bool m_IsEOF;
	bool m_IsAllocated;
	uint8* m_Data;
	uint64_t m_CurrentPosition;
	uint64_t m_BufferSize;
};