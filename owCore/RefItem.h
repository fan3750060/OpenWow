#pragma once

class RefItem
{
public:
	RefItem() : m_RefsCount(0)	{}
	virtual ~RefItem() {}

	inline void AddRef()
	{
		m_RefsCount += 1;
	}
	inline void DelRef()
	{
		if (NeedDelete())
		{
			assert1(false);
			return;
		}

		m_RefsCount -= 1;
	}

	uint32 GetRefsCount() const { return m_RefsCount; }
	bool NeedDelete() const { return (m_RefsCount <= 0); }

protected:
	uint32 m_RefsCount;
};