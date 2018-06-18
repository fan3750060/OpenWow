#include "DBC__File.h"
#pragma once

template<class RECORD_T>
inline DBCFile<RECORD_T>::DBCFile(const char* _fileName) :
	m_File(nullptr)
{
	m_FileName = string("DBFilesClient\\") + string(_fileName);
}

template<class RECORD_T>
inline DBCFile<RECORD_T>::~DBCFile()
{
	for (auto it = records.begin(); it != records.end();)
	{
		auto obj = it->second;
		it = records.erase(it);
		delete obj;
	}
}

template<class RECORD_T>
inline bool DBCFile<RECORD_T>::Open()
{
	m_File = GetManager<IFilesManager>()->Open(m_FileName);
	if (m_File == nullptr)
	{
		return false;
	}

	char header[5];
	m_File->ReadBytes(header, 4);
	header[4] = '\0';
	assert1(header[0] == 'W' && header[1] == 'D' && header[2] == 'B' && header[3] == 'C');


	m_File->ReadBytes(&recordCount, 4);// Number of records
	m_File->ReadBytes(&fieldCount, 4); // Number of fields
	m_File->ReadBytes(&recordSize, 4); // Size of a record
	m_File->ReadBytes(&stringSize, 4); // String size

	Log::Print("DBCFile[%s]: HEAD [%s], Size [%d]", m_File->Path_Name().c_str(), header, recordCount);

	assert1(fieldCount * 4 == recordSize);

	uint64_t stringTableOffset = m_File->GetPos() + recordSize * recordCount;
	stringTable = m_File->GetData() + stringTableOffset;

	// Fill record table
	for (uint64_t _offset = m_File->GetPos(); _offset != stringTableOffset; _offset += recordSize)
	{
		RECORD_T* record = new RECORD_T(this, const_cast<uint8*>(m_File->GetData() + _offset));
		records.insert(make_pair(record->Get_ID(), record));
	}

	assert1(recordCount == records.size());
	return true;
}

template <class RECORD_T>
RECORD_T* DBCFile<RECORD_T>::operator[](uint32 _id)
{
	auto recordIt = records.find(_id);
	if (recordIt != records.end())
	{
		return recordIt->second;
	}

	return nullptr;
}

template <class RECORD_T>
const map<uint32, RECORD_T*>& DBCFile<RECORD_T>::Records() const
{
	return records;
}