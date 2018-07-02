#pragma once

#pragma region DBC Builder

// Record create
#define CONCAT_GET(a) Get_##a
#define CONCAT_CLASS(a) a##Record

// Add data

#define __DBC_TVALUE(type, _name, _field) \
type CONCAT_GET(_name)() const \
{ \
	return getValue<type>(static_cast<uint32>(_field - 1)); \
}

#define __DBC_STRING(_name, _field) \
const char* CONCAT_GET(_name)() const \
{ \
	return getString(static_cast<uint32>(_field - 1)); \
}

#define __DBC_LOCSTR(_name, _field) \
string CONCAT_GET(_name)(int8 _locale = -1) const \
{ \
	return getLocalizedString(static_cast<uint32>(_field - 1)); \
}

#define __DBC_REF_ID(_dbc, _name, _field) \
_dbc##Record* CONCAT_GET(_name)() const \
{ \
	return _dbc[static_cast<uint32>(getValue<uint32>(static_cast<uint32>(_field - 1)))]; \
}

#define __DBC_TARRAY(_type, _name, _field, _size) \
_type CONCAT_GET(_name)(uint8 _index) const \
{ \
    assert1(_index < _size); \
	return getValue<_type>(static_cast<uint32>(_field - 1 + _index)); \
}

// Uses in common classes

#define __DBC_FOREIGN_KEY(_type, _dbc, _refFieldName, _dispName) \
_type __##_dispName;\
const CONCAT_CLASS(_dbc)* _dispName() \
{ \
	for (auto it = _dbc.begin(); it != _dbc.end(); ++it) \
	{ \
		if (static_cast<_type>(it->CONCAT_GET(_refFieldName)()) == __##_dispName) \
		{ \
			return &(*it); \
		} \
	} \
 \
	return nullptr; \
}

#define __DBC_FOREIGN_KEY_ID(_type, _dbc, _dispName) \
_type __##_dispName;\
const _dbc##Record* _dispName() \
{ \
    return _dbc[static_cast<uint32>(__##_dispName)]; \
}

//------------------------------------------
// Create class
//------------------------------------------

// Class begin
#define DBC_DEF_BEGIN(accessName) \
class CONCAT_CLASS(accessName) : public Record { \
public: \
	CONCAT_CLASS(accessName)() {} \
	CONCAT_CLASS(accessName)(const DBCFile<CONCAT_CLASS(accessName)>::Iterator& _iterator) : Record(_iterator->m_DBC_Stats, _iterator->m_Offset) { } \
	CONCAT_CLASS(accessName)(DBCFile<CONCAT_CLASS(accessName)>* file, const uint8* offset) : Record(file, offset) { } \
public:

// Class end
#define DBC_DEF_END \
};


//------------------------------------------
// Create accessor
//------------------------------------------

// Placed in *.cpp files
#define DBC_LOAD(accessName, fileName) DBCFile<CONCAT_CLASS(accessName)> accessName(fileName);

// Placed in *.h files
#define DBC_DEFINE(accessName) \
class CONCAT_CLASS(accessName); \
extern DBCFile<CONCAT_CLASS(accessName)> accessName;

#pragma endregion

///////////////////////////////////
// DBC STATS
///////////////////////////////////
class Record;
class DBCStats
{
	friend Record;
public:
	uint32_t getRecordSize() const { return recordSize; }
	uint32_t getRecordCount() const { return recordCount; }
	uint32_t getFieldCount() const { return fieldCount; }
	uint32_t getStringSize() const { return stringSize; }

protected:
	uint32_t recordSize;
	uint32_t recordCount;
	uint32_t fieldCount;
	uint32_t stringSize;

	// Strings
	const uint8* stringTable;
};

///////////////////////////////////
// Record type
///////////////////////////////////
class Record
{
public:
	Record() : m_DBC_Stats(nullptr), m_Offset(nullptr) {}
	Record(const Record& _record) : m_DBC_Stats(_record.m_DBC_Stats), m_Offset(_record.m_Offset) {}
	Record(Record&& _record) = delete;
	Record(const DBCStats* _dbcStats, const uint8* offset) : m_DBC_Stats(_dbcStats), m_Offset(offset) {}

	Record& operator=(const Record& r)
	{
		m_DBC_Stats = r.m_DBC_Stats;
		m_Offset = r.m_Offset;
		return *this;
	}
	Record& operator=(Record&& r) = delete;

	const DBCStats*	getDBCStats() const { return m_DBC_Stats; }
	void incOffset(uint32 _value) { m_Offset += _value; }
	const uint8*	getOffset() const { return m_Offset; }

	// All data has ID
	__DBC_TVALUE(uint32, ID, 1);

protected:
	// Get value with common type
	template<typename T>
	T getValue(uint32 field) const
	{
		assert2(field < m_DBC_Stats->getFieldCount(), std::to_string(field).c_str());
		return *reinterpret_cast<T*>(const_cast<uint8*>(m_Offset) + field * 4);
	}

	// Strings
	const char* getString(uint32 field) const
	{
		assert2(field < m_DBC_Stats->getFieldCount(), std::to_string(field).c_str());

		uint32_t stringOffset = getValue<uint32>(field);
		if (stringOffset >= m_DBC_Stats->getStringSize())
		{
			stringOffset = 0;
		}

		assert2(stringOffset < m_DBC_Stats->getStringSize(), std::to_string(stringOffset).c_str());
		return reinterpret_cast<char*>(const_cast<uint8*>(m_DBC_Stats->stringTable) + stringOffset);
	}

	string getLocalizedString(uint32 field, int8 locale = -1) const
	{
		int8 loc = locale;
		if (locale == -1)
		{
			assert2(field < m_DBC_Stats->getFieldCount() - 16, std::to_string(field).c_str());
			for (loc = 0; loc < 16; loc++)
			{
				uint32 stringOffset = getValue<uint32>(field + loc);
				if (stringOffset != 0)
				{
					break;
				}
			}
		}
		assert2(field + loc < m_DBC_Stats->getFieldCount(), std::to_string(field).c_str());
		uint32 stringOffset = getValue<uint32>(field + static_cast<uint32>(loc));

		assert2(stringOffset < m_DBC_Stats->getStringSize(), std::to_string(stringOffset).c_str());
		return Utf8_to_cp1251(reinterpret_cast<char*>(const_cast<uint8*>(m_DBC_Stats->stringTable) + stringOffset));
	}

protected:
	const DBCStats*		m_DBC_Stats;
	const uint8*		m_Offset;
};



///////////////////////////////////
// DBC File
///////////////////////////////////
class File;
template <class RECORD_T>
class DBCFile : public DBCStats
{
	friend RECORD_T;
public:
	DBCFile(const char* _fileName);
	~DBCFile();

	bool Open();

	// Get data by id
	RECORD_T* operator[](uint32 _id);

	// Iterator that iterates over records
	class Iterator
	{
		friend RECORD_T;
	public:
		Iterator(DBCFile* file, const uint8* offset) : m_Record(file, offset) {}
		Iterator(Iterator& _iterator) : m_Record(_iterator.m_Record) {}

		Iterator& operator++()
		{
			m_Record.incOffset(m_Record.getDBCStats()->getRecordSize());
			return *this;
		}

		const RECORD_T& operator*() const { return m_Record; }
		const RECORD_T* operator->() const { return &m_Record; }

		bool operator==(const Iterator &b) const { return m_Record.getOffset() == b.m_Record.getOffset(); }
		bool operator!=(const Iterator &b) const { return m_Record.getOffset() != b.m_Record.getOffset(); }

		RECORD_T get() const { return m_Record; }

	private:
		RECORD_T m_Record;
	};

	Iterator begin()
	{
		assert1(m_File->getData() != nullptr);
		return Iterator(this, m_File->getData() + m_File->getPos());
	}

	Iterator end()
	{
		assert1(m_File->getData() != nullptr);
		return Iterator(this, stringTable);
	}

	const multimap<uint32, RECORD_T*>& Records() const;

protected:
	multimap<uint32, RECORD_T*> records;

private:
	string m_FileName;
	IFile* m_File;
};

#include "DBC__File.inl"