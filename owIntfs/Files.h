#pragma once

struct
	__declspec(novtable)
	IByteBuffer
{
	virtual size_t getSize() const = 0;
	virtual size_t getPos() const = 0;
	virtual const uint8* getData() const = 0;
	virtual const uint8* getDataFromCurrent() const = 0;
	virtual bool isEof() const = 0;

	virtual void seek(size_t _bufferOffsetAbsolute) = 0;
	virtual void seekRelative(intptr_t _bufferOffsetRelative) = 0;
	virtual bool readLine(std::string* _string) = 0;
	virtual void readBytes(void* _destination, size_t _size = 1) = 0;
	virtual void readString(std::string* _string) = 0;
};


struct
	__declspec(novtable)
	IFile : public IByteBuffer
{
	virtual ~IFile() {}

	virtual bool Open() = 0;

	virtual std::string Name() const = 0;
	virtual std::string Path() const = 0;
	virtual std::string Extension() const = 0;
	virtual std::string Path_Name() const = 0;
};

struct 
	__declspec(novtable, uuid("5DC32EB8-9A63-4FAD-A4BF-81916B8EF86A"))
	IFilesManager : public IManager
{
	virtual ~IFilesManager() {}

	virtual std::shared_ptr<IFile> Open(cstring _fileName) = 0;
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
	__declspec(novtable, uuid("6658FDFF-48AB-4712-8FAE-A1C32DFF8815"))
	IMPQArchiveManager : public IManager
{
	virtual ~IMPQArchiveManager() {};

	virtual void AddArchive(std::string _filename) = 0;
	virtual SMPQFileLocation GetFileLocation(cstring _filename) = 0;

	virtual std::mutex& Guard() = 0;
};