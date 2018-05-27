#pragma once

template<class T>
struct DefaultDelete
{
	void operator()(T* _Ptr) const
	{
		static_assert(0 < sizeof(T), "can't delete an incomplete type");
		delete _Ptr;
	}
};

template<class T, class DEL = DefaultDelete<T>>
class SmartPtr
{
public:
	SmartPtr(T* ptr = nullptr) 
	{
		m_Ptr = ptr;
		AddRef();
	}
	SmartPtr(const SmartPtr& _other)
	{
		m_Ptr = _other.m_Ptr;
		AddRef();
	}
	~SmartPtr()
	{
		Release();
	}

	T& operator*() const { return *m_Ptr; }
	T* operator->() const { return m_Ptr; }
	operator T*() const { return m_Ptr; }
	operator const T*() const { return m_Ptr; }
	operator bool() const { return m_Ptr != nullptr; }

	bool operator==(T* _other)	{		return m_Ptr == _other;	}
	bool operator!=(T* _other)	{		return m_Ptr != _other;	}

	SmartPtr& operator=(const SmartPtr& _other)
	{
		m_Ptr = _other.m_Ptr;
		AddRef();
		return *this;
	}
	SmartPtr& operator=(T* ptr)
	{
		Release();
		m_Ptr = ptr;
		AddRef();
		return *this;
	}

private:
	void AddRef() const
	{
		if (m_Ptr != nullptr)
		{
			m_Ptr->AddRef();
		}
	}
	void Release() const
	{
		if (m_Ptr != nullptr)
		{
			m_Ptr->Release();
			assert1(m_Ptr->GetRefsCount() >= 0);
			if (m_Ptr->GetRefsCount() == 0)
			{
				DEL deleter;
				deleter.operator()(m_Ptr);
			}
		}
	}

private:
	T* m_Ptr;
};