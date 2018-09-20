#include <stdafx.h>

#include "QueryOGL.h"

// Additional
#include "OpenGL.h"

QueryOGL::QueryOGL(QueryType queryType, uint8_t numBuffers)
	: m_QueryType(queryType)
	, m_NumBuffers(numBuffers)
{
	throw std::exception("Not implemented!");
}

QueryOGL::~QueryOGL()
{}

void QueryOGL::Begin(int64_t frame)
{
	throw std::exception("Not implemented!");
}

void QueryOGL::End(int64_t frame)
{
	throw std::exception("Not implemented!");
}

bool QueryOGL::QueryResultAvailable(int64_t frame)
{
	throw std::exception("Not implemented!");
}

Query::QueryResult QueryOGL::GetQueryResult(int64_t frame)
{
	throw std::exception("Not implemented!");
}

uint8_t QueryOGL::GetBufferCount() const
{
	return m_NumBuffers;
}
