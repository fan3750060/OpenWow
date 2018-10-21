#pragma once

__interface IByteBuffer
{
	uint64 getSize() const;
	uint64 getPos() const;
	const uint8* getData() const;
	const uint8* getDataFromCurrent() const;
	bool isEof() const;

	void seek(uint64 _bufferOffsetAbsolute);
	void seekRelative(uint64 _bufferOffsetRelative);
	bool readLine(std::string* _string);
	void readBytes(void* _destination, uint64 _size = 1);
	void readString(std::string* _string);
};


struct IFile : public IByteBuffer
{
	virtual ~IFile() {}

	virtual bool Open() = 0;

	virtual std::string Name() const = 0;
	virtual std::string Path() const = 0;
	virtual std::string Extension() const = 0;
	virtual std::string Path_Name() const = 0;
};

struct 
	__declspec(uuid("5DC32EB8-9A63-4FAD-A4BF-81916B8EF86A"))
	IFilesManager : public IManager
{
	virtual ~IFilesManager() {}

	virtual std::shared_ptr<IFile> Open(cstring _fileName) = 0;

	virtual void Lock() = 0;
	virtual void Unlock() = 0;
	virtual void Guard() = 0;
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

struct
	__declspec(uuid("6658FDFF-48AB-4712-8FAE-A1C32DFF8815"))
	IMPQArchiveManager : public IManager
{
	virtual ~IMPQArchiveManager() {};

	virtual void AddArchive(std::string _filename) = 0;
	virtual SMPQFileLocation GetFileLocation(cstring _filename) = 0;
};