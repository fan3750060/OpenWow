#pragma once

#include "RefItem.h"
#include "ByteBuffer.h"

class CFile : public CRefItem, public IFile
{
public:
	 CFile(cstring _fullFileName);
	 CFile(cstring _name, cstring _path);
	 ~CFile();

	 // IRefItem
	 void AddRef() override { CRefItem::AddRef(); }
	 void Release() override { CRefItem::Release(); }
	 uint32 GetRefsCount() const { return CRefItem::GetRefsCount();
	 }
	 // IFile
	 inline string Name() const override { return m_Name; }
	 inline string Path() const override { return m_Path; }
	 inline string Extension() const override { return m_Extension; }
	 inline string Path_Name() const override { return string(m_Path + m_Name); }

	 // IByteBuffer
	 inline uint64_t GetSize() const override { return m_ByteBuffer.GetSize(); }
	 inline uint64_t GetPos() const override { return m_ByteBuffer.GetPos(); }
	 inline const uint8* GetData() const override { return m_ByteBuffer.GetData(); }
	 inline const uint8* GetDataFromCurrent() const override { return m_ByteBuffer.GetDataFromCurrent(); }
	 inline bool IsEof() const override { return m_ByteBuffer.IsEof(); }
	 inline void Seek(uint64_t _bufferOffsetAbsolute) override { m_ByteBuffer.Seek(_bufferOffsetAbsolute); }
	 inline void SeekRelative(uint64_t _bufferOffsetRelative) override { m_ByteBuffer.SeekRelative(_bufferOffsetRelative); }
	 inline string ReadLine() override { return m_ByteBuffer.ReadLine(); }
	 inline void ReadBytes(void* _destination, uint64_t _size) override { m_ByteBuffer.ReadBytes(_destination, _size); }

public:
	static void FixFilePath(string& _string);

private:
	void ParsePathAndExtension();

protected:
	ByteBuffer m_ByteBuffer;

private:
	string m_Name;
	string m_Path;
	string m_Extension;
};