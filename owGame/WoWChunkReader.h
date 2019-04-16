#pragma once

class WoWChunkReader 
{
public:
	WoWChunkReader(std::string _fileName);
	WoWChunkReader(std::shared_ptr<IFile> file);
	virtual ~WoWChunkReader();

	std::shared_ptr<IByteBuffer> OpenChunk(const char* _name);
	std::vector<std::shared_ptr<IByteBuffer>> OpenChunks(const char* _name);

	template<class T>
	std::vector<T> OpenChunkT(const char* _name);

private:
	typedef std::pair<size_t, size_t>  ChunkInfo;
	typedef std::vector<ChunkInfo>     Chunks;

protected:
	void InitMaps();
	std::shared_ptr<IByteBuffer> GetChunk(ChunkInfo chunks);

private:
	std::shared_ptr<IFile> m_File;
	std::map<std::string, Chunks> m_ChunksMap; // seek and size
};

#include "WoWChunkReader.inl"
