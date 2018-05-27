#pragma once

class RefItem
{
public:
	RefItem() : 
		m_RefsCount(0),
		m_IsDeleted(false)
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

	void setDeleted() { m_IsDeleted = true; }
	bool getDeleted() { return m_IsDeleted; }

private:
	uint32 m_RefsCount;
	bool m_IsDeleted;
};