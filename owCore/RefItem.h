#pragma once

class RefItem
{
public:
	RefItem() : 
		m_RefsCount(0)
	{}

	inline void AddRef()
	{
		m_RefsCount += 1;
	}
	inline void Release()
	{
		m_RefsCount -= 1;
	}

	uint32 GetRefsCount() const { return m_RefsCount; }

private:
	uint32 m_RefsCount;

};