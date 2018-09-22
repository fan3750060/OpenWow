#pragma once

#include "ByteBuffer.h"

class CFile : public IFile
{
public:
	 CFile(cstring _fullFileName);
	 CFile(cstring _name, cstring _path);
	 virtual ~CFile();

	 // IFile
	 string Name() const override { return m_Name; }
	 string Path() const override { return m_Path; }
	 string Extension() const override { return m_Extension; }
	 string Path_Name() const override { return string(m_Path + m_Name); }

	 // IByteBuffer
	 uint64_t getSize() const override 
	 { 
		 m_FilesManager->Guard();
		 return m_ByteBuffer.getSize(); 
	 }
	 uint64_t getPos() const override 
	 { 
		 m_FilesManager->Guard();
		 return m_ByteBuffer.getPos(); 
	 }
	 const uint8* getData() const override 
	 { 
		 m_FilesManager->Guard();
		 return m_ByteBuffer.getData(); 
	 }
	 const uint8* getDataFromCurrent() const override 
	 { 
		 m_FilesManager->Guard();
		 return m_ByteBuffer.getDataFromCurrent(); 
	 }
	 bool isEof() const override 
	 { 
		 m_FilesManager->Guard();
		 return m_ByteBuffer.isEof(); 
	 }
	 void seek(uint64_t _bufferOffsetAbsolute) override 
	 { 
		 m_FilesManager->Guard();
		 m_ByteBuffer.seek(_bufferOffsetAbsolute); 
	 }
	 void seekRelative(uint64_t _bufferOffsetRelative) override 
	 { 
		 m_FilesManager->Guard();
		 m_ByteBuffer.seekRelative(_bufferOffsetRelative); 
	 }
	 bool readLine(string* _string) override
	 { 
		 m_FilesManager->Guard();
		 return m_ByteBuffer.readLine(_string);
	 }
	 void readBytes(void* _destination, uint64_t _size) override 
	 { 
		 m_FilesManager->Guard();
		 m_ByteBuffer.readBytes(_destination, _size); 
	 }
	 void readString(string* _string) override
	 {
		 m_FilesManager->Guard();
		 m_ByteBuffer.readString(_string);
	 }

public:
	static void FixFilePath(string& _string);

private:
	void ParsePathAndExtension();

protected:
	ByteBuffer m_ByteBuffer;
	IFilesManager* m_FilesManager;

private: // IFile
	string m_Name;
	string m_Path;
	string m_Extension;
};