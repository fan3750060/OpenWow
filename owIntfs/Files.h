#pragma once


__interface IByteBuffer
{
	uint64_t GetSize() const;
	uint64_t GetPos() const;
	const uint8* GetData() const;
	const uint8* GetDataFromCurrent() const;
	bool IsEof() const;

	void Seek(uint64_t _bufferOffsetAbsolute);
	void SeekRelative(uint64_t _bufferOffsetRelative);
	string ReadLine();
	void ReadBytes(void* _destination, uint64_t _size);
};

__interface IFile : public IByteBuffer
{
	bool Open();

	string Name() const;
	string Path() const;
	string Extension() const;
	string Path_Name() const;
};

__interface IFilesManager
{
	IFile* Open(cstring _fileName);
};