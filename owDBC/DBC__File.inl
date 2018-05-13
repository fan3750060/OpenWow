#pragma once

template <class RECORD_T>
bool DBCFile<RECORD_T>::Open()
{
	// Try open file
	if (!File::Open())
	{
		Log::Error("DBCFile[%s]: Can't open file.", Path_Name().c_str());
		return false;
	}

	char header[5];
	ByteBuffer::ReadBytes(header, 4);
	header[4] = '\0';
	assert1(header[0] == 'W' && header[1] == 'D' && header[2] == 'B' && header[3] == 'C');


	ByteBuffer::ReadBytes(&recordCount, 4);// Number of records
	ByteBuffer::ReadBytes(&fieldCount, 4); // Number of fields
	ByteBuffer::ReadBytes(&recordSize, 4); // Size of a record
	ByteBuffer::ReadBytes(&stringSize, 4); // String size

	Log::Print("DBCFile[%s]: HEAD [%s], Size [%d]", Path_Name().c_str(), header, recordCount);

	assert1(fieldCount * 4 == recordSize);

	uint64_t stringTableOffset = GetPos() + recordSize * recordCount;
	stringTable = data + stringTableOffset;

	// Fill record table
	for (uint64_t _offset = GetPos(); _offset != stringTableOffset; _offset += recordSize)
	{
		RECORD_T* record = new RECORD_T(this, data + _offset);
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