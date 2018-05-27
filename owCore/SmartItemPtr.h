#pragma once

template<class T>
class SmartItemPtr
{
public:
	SmartItemPtr(T* ptr = nullptr) : 
		m_Ptr(ptr)
	{
		AddRef();
	}
	SmartItemPtr(SmartItemPtr& smp) : 
		m_Ptr(smp.m_Ptr)
	{
		AddRef();
	}
	~SmartItemPtr()
	{
		Release();
	}

	T& operator*() const { return *m_Ptr; }
	T* operator->() const { return m_Ptr; }
	operator T*() const { return m_Ptr; }
	operator const T*() const { return m_Ptr; }
	operator bool() const { return m_Ptr != nullptr; }

	bool operator==(T* _other)
	{
		return m_Ptr == _other;
	}
	bool operator!=(T* _other)
	{
		return m_Ptr != _other;
	}

	SmartItemPtr& operator=(const SmartItemPtr& _other)
	{
		AddRef();
		return *this;
	}
	SmartItemPtr& operator=(T* ptr)
	{
		Release();
		m_Ptr = ptr;
		AddRef();
		return *this;
	}

private:
	void AddRef()
	{
		if (m_Ptr != nullptr)
		{
			m_Ptr->AddRef();
		}
	}
	void Release()
	{
		if (m_Ptr != nullptr)
		{
			m_Ptr->Release();
			if (m_Ptr->GetRefsCount() == 0)
			{
				delete m_Ptr;
			}
		}
	}

private:
	T* m_Ptr;
};