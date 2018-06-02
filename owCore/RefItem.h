#pragma once

class CRefItem
{
public:
	CRefItem() :
		m_RefsCount(0)
	{}

	void AddRef() { m_RefsCount += 1; }
	void Release() { m_RefsCount -= 1; }
	uint32 GetRefsCount() const { return m_RefsCount; }

private:
	uint32 m_RefsCount;

};