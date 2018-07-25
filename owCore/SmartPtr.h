#pragma once

template<class T>
struct DefaultDeleter
{
	void operator()(T* _Ptr) const
	{
		static_assert(0 < sizeof(T), "can't delete an incomplete type");
		delete _Ptr;
		_Ptr = nullptr;
	}
};

template<class T, class D = DefaultDeleter<T>>
class SharedPtr
{
public:
	SharedPtr(T* ptr = nullptr) 
	{
		m_Ptr = ptr;
		AddRef();
	}
	SharedPtr(const SharedPtr& _other)
	{
		m_Ptr = _other.m_Ptr;
		AddRef();
	}
	/*SharedPtr(SharedPtr&& _other)
	{
		m_Ptr = _other.m_Ptr;
		AddRef();
	}*/
	~SharedPtr()
	{
		Release();
	}

	T& operator*() const { return *m_Ptr; }
	T* operator->() const { return m_Ptr; }
	operator T*() const { return m_Ptr; }
	operator const T*() const { return m_Ptr; }
	operator bool() const { return m_Ptr != nullptr; }
	T* getPtr() const { return m_Ptr; }

	bool operator==(T* _other)	{		return m_Ptr == _other;	}
	bool operator!=(T* _other)	{		return m_Ptr != _other;	}

	SharedPtr& operator=(const SharedPtr& _other)
	{
		m_Ptr = _other.m_Ptr;
		AddRef();
		return *this;
	}
	/*SharedPtr& operator=(SharedPtr&& _other)
	{
		m_Ptr = _other.m_Ptr;
		AddRef();
		return *this;
	}*/
	SharedPtr& operator=(T* ptr)
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
				D m_Deleter;
				(m_Deleter).operator()(m_Ptr);
			}
		}
	}

private:
	T* m_Ptr;
};