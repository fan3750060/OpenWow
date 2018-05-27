#pragma once

template<class T>
class UniquePtr
{
public:
	UniquePtr(T* ptr = nullptr) :
		m_Ptr(ptr)
	{}
	UniquePtr(UniquePtr& smp) :
		m_Ptr(smp.m_Ptr)
	{}
	~UniquePtr()
	{
		delete m_Ptr;
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

	UniquePtr& operator=(const UniquePtr& _other)
	{
		return *this;
	}
	UniquePtr& operator=(T* ptr)
	{
		delete m_Ptr;
		m_Ptr = ptr;
		return *this;
	}

private:
	T* m_Ptr;
};