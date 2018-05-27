#pragma once


__interface IByteBuffer
{
	uint64 GetSize() const;
	uint64 GetPos() const;
	const uint8* GetData() const;
	const uint8* GetDataFromCurrent() const;
	bool IsEof() const;

	void Seek(uint64 _bufferOffsetAbsolute);
	void SeekRelative(uint64 _bufferOffsetRelative);
	string ReadLine();
	void ReadBytes(void* _destination, uint64 _size);
};

__interface IFile : public IByteBuffer
{
	bool Open();

	string Name() const;
	string Path() const;
	string Extension() const;
	string Path_Name() const;
};

__interface IFilesManager : public IManager
{
	IFile* Open(cstring _fileName);
};