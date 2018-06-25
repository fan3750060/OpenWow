#pragma once

class ByteBufferOutput
{
public:
	void Write(int8 _data);
	void Write(int16 _data);
	void Write(int32 _data);
	void Write(int64 _data);

	void Write(uint8 _data);
	void Write(uint16 _data);
	void Write(uint32 _data);
	void Write(uint64 _data);

	void Write(const uint8* _string, uint32 _size);

	void Write(cstring _string, uint32 _expectedSize = UINT32_MAX);

	void WriteDummy(uint32 _size);


public:
	const uint8* getData() const { return m_Data.data(); }
	uint32 getSize() const { return static_cast<uint32>(m_Data.size()); }

private:
	void Append(const uint8* _data, uint32 size);

private:
	vector<uint8> m_Data;
};