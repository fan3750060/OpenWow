#pragma once

#include "Refs.h"

__interface IByteBuffer
{
	uint64 getSize() const;
	uint64 getPos() const;
	const uint8* getData() const;
	const uint8* getDataFromCurrent() const;
	bool isEof() const;

	void seek(uint64 _bufferOffsetAbsolute);
	void seekRelative(uint64 _bufferOffsetRelative);
	string readLine();
	void readBytes(void* _destination, uint64 _size);
};


__interface IByteBufferOutput
{
	const uint8* getData() const;
	uint32 getSize() const;
};

struct IFile : public IByteBuffer, public IRefItem
{
	virtual ~IFile() {}

	virtual bool Open() = 0;

	virtual string Name() const = 0;
	virtual string Path() const = 0;
	virtual string Extension() const = 0;
	virtual string Path_Name() const = 0;
};

__interface 
	__declspec(uuid("5DC32EB8-9A63-4FAD-A4BF-81916B8EF86A"))
	IFilesManager : public IManager
{
	IFile* Open(cstring _fileName);

	void Lock();
	void Unlock();
	void Guard();
};

// FORWARD BEGIN
struct mpq_archive;
// FORWARD END

struct SMPQFileLocation
{
	SMPQFileLocation() : 
		archive(nullptr), 
		fileNumber(0), 
		exists(false) 
	{}

	SMPQFileLocation(mpq_archive* _archive, uint32 _fileNumber) : 
		archive(_archive), 
		fileNumber(_fileNumber), 
		exists(true) 
	{}

	bool exists;
	mpq_archive* archive;
	uint32 fileNumber;
};

__interface
	__declspec(uuid("6658FDFF-48AB-4712-8FAE-A1C32DFF8815"))
	IMPQArchiveManager : public IManager
{
	void AddArchive(string _filename);
	SMPQFileLocation GetFileLocation(cstring _filename);
};