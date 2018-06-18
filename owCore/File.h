#pragma once

#include "RefItem.h"
#include "ByteBuffer.h"

class CFile : public IFile
{
public:
	 CFile(cstring _fullFileName);
	 CFile(cstring _name, cstring _path);
	 virtual ~CFile();

	 // IRefItem
	 /*void AddRef() override { CRefItem::AddRef(); }
	 void Release() override { CRefItem::Release(); }
	 uint32 GetRefsCount() const { return CRefItem::GetRefsCount();	 }*/
	 void AddRef() override { m_RefsCount += 1; }
	 void Release() override { m_RefsCount -= 1; }
	 uint32 GetRefsCount() const override { return m_RefsCount; }

	 // IFile
	 string Name() const override { return m_Name; }
	 string Path() const override { return m_Path; }
	 string Extension() const override { return m_Extension; }
	 string Path_Name() const override { return string(m_Path + m_Name); }

	 // IByteBuffer
	 uint64_t GetSize() const override { return m_ByteBuffer.GetSize(); }
	 uint64_t GetPos() const override { return m_ByteBuffer.GetPos(); }
	 const uint8* GetData() const override { return m_ByteBuffer.GetData(); }
	 const uint8* GetDataFromCurrent() const override { return m_ByteBuffer.GetDataFromCurrent(); }
	 bool IsEof() const override { return m_ByteBuffer.IsEof(); }
	 void Seek(uint64_t _bufferOffsetAbsolute) override { m_ByteBuffer.Seek(_bufferOffsetAbsolute); }
	 void SeekRelative(uint64_t _bufferOffsetRelative) override { m_ByteBuffer.SeekRelative(_bufferOffsetRelative); }
	 string ReadLine() override { return m_ByteBuffer.ReadLine(); }
	 void ReadBytes(void* _destination, uint64_t _size) override { m_ByteBuffer.ReadBytes(_destination, _size); }

public:
	static void FixFilePath(string& _string);

private:
	void ParsePathAndExtension();

protected:
	ByteBuffer m_ByteBuffer;

private: // IFile
	string m_Name;
	string m_Path;
	string m_Extension;

private: // IRefItem
	uint32 m_RefsCount;
};