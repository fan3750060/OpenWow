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
const char* CONCAT_GET(_name)(int8 _locale = -1) const \
{ \
	return getString(static_cast<uint32>(_field - 1)); \
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
	CONCAT_CLASS(accessName)(const DBCFile<CONCAT_CLASS(accessName)>::Iterator& _iterator) : Record(_iterator->dbcStats, _iterator->offset) { } \
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
	Record() = delete;
	Record(const Record& _record) = delete;
	Record(const DBCStats* _dbcStats, const uint8* offset) : dbcStats(_dbcStats), offset(offset) {}

	Record& operator=(const Record& r) = delete;

	// All data has ID
	__DBC_TVALUE(uint32, ID, 1);

protected:
	// Get value with common type
	template<typename T>
	T getValue(uint32 field) const
	{
		assert2(field < dbcStats->fieldCount, std::to_string(field).c_str());
		return *reinterpret_cast<T*>(const_cast<uint8*>(offset) + field * 4);
	}

	// Strings
	const char* getString(uint32 field) const
	{
		assert2(field < dbcStats->fieldCount, std::to_string(field).c_str());

		uint32_t stringOffset = getValue<uint32>(field);
		if (stringOffset >= dbcStats->stringSize)
		{
			stringOffset = 0;
		}

		assert2(stringOffset < dbcStats->stringSize, std::to_string(stringOffset).c_str());
		return reinterpret_cast<char*>(const_cast<uint8*>(dbcStats->stringTable) + stringOffset);
	}
	const char* getLocalizedString(uint32 field, int8 locale = -1) const
	{
		int8 loc = locale;
		if (locale == -1)
		{
			assert2(field < dbcStats->fieldCount - 8, std::to_string(field).c_str());
			for (loc = 0; loc < 8; loc++)
			{
				uint32 stringOffset = getValue<uint32>(field + loc);
				if (stringOffset != 0)
				{
					break;
				}
			}
		}
		assert2(field + loc < dbcStats->fieldCount, std::to_string(field).c_str());
		uint32 stringOffset = getValue<uint32>(field + static_cast<uint32>(loc));

		assert2(stringOffset < dbcStats->stringSize, std::to_string(stringOffset).c_str());
		return reinterpret_cast<char*>(const_cast<uint8*>(dbcStats->stringTable) + stringOffset);
	}

public:
	const DBCStats* dbcStats;
	const uint8* offset;
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
	~DBCFile()
	{
		for (auto it = records.begin(); it != records.end();)
		{
			auto obj = it->second;
			it = records.erase(it);
			delete obj;
		}
	}

	bool Open();

	// Get data by id
	RECORD_T* operator[](uint32 _id);

	///////////////////////////////////
	// Iterator that iterates over records
	///////////////////////////////////
	class Iterator
	{
		friend RECORD_T;
	public:
		Iterator(DBCFile* file, const uint8* offset) : record(file, offset) {}
		Iterator(Iterator& _iterator) : record(_iterator.record) {}

		Iterator& operator++()
		{
			record.offset += record.dbcStats->getRecordSize();
			return *this;
		}

		const RECORD_T& operator*() const
		{
			return record;
		}
		const RECORD_T* operator->() const
		{
			return &record;
		}

		bool operator==(const Iterator &b) const
		{
			return record.offset == b.record.offset;
		}
		bool operator!=(const Iterator &b) const
		{
			return record.offset != b.record.offset;
		}

	private:
		RECORD_T record;		
	};

	// Iterators
	Iterator begin()
	{
		assert1(m_File->GetData() != nullptr);
		return Iterator(this, m_File->GetData() + m_File->GetPos());
	}

	Iterator end()
	{
		assert1(m_File->GetData() != nullptr);
		return Iterator(this, stringTable);
	}

	const map<uint32, RECORD_T*>& Records() const
	{
		return records;
	}

protected:
	map<uint32, RECORD_T*> records;

private:
	string m_FileName;
	IFile* m_File;
};

#include "DBC__File.inl"