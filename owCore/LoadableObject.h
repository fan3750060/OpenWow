#pragma once

class CLoadableObject : public ILoadable
{
public:
	CLoadableObject() :
		m_IsLoaded(false)
	{}

	// ILoadableObject
	bool IsLoaded() const override { return m_IsLoaded; }
	void SetLoaded() override { m_IsLoaded = true; }

private:
	bool m_IsLoaded;
};