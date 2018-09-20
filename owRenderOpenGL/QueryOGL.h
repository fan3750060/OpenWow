#pragma once

#include "Query.h"

class QueryOGL : public Query
{
public:

	QueryOGL(QueryType queryType, uint8_t numBuffers);
	virtual ~QueryOGL();

	virtual void Begin(int64_t frame = 0L);
	virtual void End(int64_t frame = 0L);
	virtual bool QueryResultAvailable(int64_t frame = 0L);
	virtual QueryResult GetQueryResult(int64_t frame = 0L);
	virtual uint8_t GetBufferCount() const;

protected:

private:
	//typedef std::vector< ATL::CComPtr<ID3D11Query> > QueryBuffer;
	//QueryBuffer m_DisjointQueries;
	// For timer queries, we need 2 sets of buffered queries.
	//QueryBuffer m_Queries[2];

	QueryType m_QueryType;
	// How many queries will be used to prevent stalling the GPU.
	uint8_t m_NumBuffers;
};