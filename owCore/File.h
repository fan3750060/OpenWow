#pragma once

#include "ByteBuffer.h"

class CFile : public IFile
{
public:
	 CFile(cstring _fullFileName);
	 CFile(cstring _name, cstring _path);
	 virtual ~CFile();

	 // IFile
	 std::string Name() const override { return m_Name; }
	 std::string Path() const override { return m_Path; }
	 std::string Extension() const override { return m_Extension; }
	 std::string Path_Name() const override { return std::string(m_Path + m_Name); }

	 // IByteBuffer
	 uint64_t getSize() const override 
	 { 
		 return m_ByteBuffer.getSize(); 
	 }
	 uint64_t getPos() const override 
	 { 
		 return m_ByteBuffer.getPos(); 
	 }
	 const uint8* getData() const override 
	 { 
		 return m_ByteBuffer.getData(); 
	 }
	 const uint8* getDataFromCurrent() const override 
	 { 
		 return m_ByteBuffer.getDataFromCurrent(); 
	 }
	 bool isEof() const override 
	 { 
		 return m_ByteBuffer.isEof(); 
	 }
	 void seek(uint64_t _bufferOffsetAbsolute) override 
	 { 
		 m_ByteBuffer.seek(_bufferOffsetAbsolute); 
	 }
	 void seekRelative(uint64_t _bufferOffsetRelative) override 
	 { 
		 m_ByteBuffer.seekRelative(_bufferOffsetRelative); 
	 }
	 bool readLine(std::string* _string) override
	 { 
		 return m_ByteBuffer.readLine(_string);
	 }
	 void readBytes(void* _destination, uint64_t _size) override 
	 { 
		 m_ByteBuffer.readBytes(_destination, _size); 
	 }
	 void readString(std::string* _string) override
	 {
		 m_ByteBuffer.readString(_string);
	 }

public:
	static void FixFilePath(std::string& _string);

private:
	void ParsePathAndExtension();

protected:
	ByteBuffer m_ByteBuffer;
	std::shared_ptr<IFilesManager> m_FilesManager;

private: // IFile
	std::string m_Name;
	std::string m_Path;
	std::string m_Extension;
};