#pragma once

template<class RECORD_T>
inline DBCFile<RECORD_T>::DBCFile(const char* _fileName) :
	m_File(nullptr)
{
	m_FileName = std::string("DBFilesClient\\") + std::string(_fileName);
}

template<class RECORD_T>
inline DBCFile<RECORD_T>::~DBCFile()
{
	m_Records.clear();
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
	m_File->readBytes(header, 4);
	header[4] = '\0';

#if (VERSION == VERSION_Cata)
	int db_type = 0;
	if (strncmp(header, "WDBC", 4) == 0)
		db_type = 1;
	else if (strncmp(header, "WDB2", 4) == 0)
		db_type = 2;
	else
	{
		Log::Error("DBCFile[%s]: File corrupt. Header [%s]", m_File->Path_Name().c_str(), header);
		return false;
	}
#endif

	m_File->readBytes(&recordCount, 4);// Number of records
	m_File->readBytes(&fieldCount, 4); // Number of fields
	m_File->readBytes(&recordSize, 4); // Size of a record
	m_File->readBytes(&stringSize, 4); // String size

	Log::Info("DBCFile[%s]: HEAD [%s], Size [%d]", m_File->Path_Name().c_str(), header, recordCount);

#if (VERSION == VERSION_Cata)
	if (db_type == 2)
	{
		m_File->seekRelative(28);

		unsigned int check;
		m_File->readBytes(&check, 4);
		if (check == 6)
		{
			m_File->seekRelative(-20);
		}
		else // 17
		{
			m_File->seekRelative(-4);
		}
	}
#endif

	assert1(fieldCount * 4 == recordSize);

	uint64_t stringTableOffset = m_File->getPos() + recordSize * recordCount;
	stringTable = m_File->getData() + stringTableOffset;

	// Fill record table
	for (uint64_t _offset = m_File->getPos(); _offset != stringTableOffset; _offset += recordSize)
	{
		std::shared_ptr<RECORD_T> record = std::make_shared<RECORD_T>(this, const_cast<uint8*>(m_File->getData() + _offset));
		m_Records.insert(std::make_pair(record->Get_ID(), record));
	}

	assert1(recordCount == m_Records.size());
	return true;
}

template <class RECORD_T>
std::shared_ptr<RECORD_T> DBCFile<RECORD_T>::operator[](uint32 _id)
{
	auto recordIt = m_Records.find(_id);
	if (recordIt != m_Records.end())
	{
		return recordIt->second;
	}

	return nullptr;
}

template <class RECORD_T>
const std::multimap<uint32, std::shared_ptr<RECORD_T>>& DBCFile<RECORD_T>::Records() const
{
	return m_Records;
}