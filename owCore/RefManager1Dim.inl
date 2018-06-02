#pragma once

template<class T>
RefManager1Dim<T>::RefManager1Dim()
{
}

template<class T>
RefManager1Dim<T>::~RefManager1Dim()
{
	for (auto it = objects.begin(); it != objects.end(); )
	{
		T* item = it->second;
		string name = GetNameByItem(item);

		this->DeleteAction(name);
		delete item;
		it = objects.erase(it);
		item = nullptr;
	}
}

template <class T>
T* RefManager1Dim<T>::Add(cstring name)
{
	T* item = GetItemByName(name);
	if (item != nullptr)
	{
		return item;
	}

	item = CreateAction(name);
	objects[name] = item;

	return item;
}

template<class T>
bool RefManager1Dim<T>::Exists(cstring name) const
{
	return objects.find(name) != objects.end();
}

template <class T>
void RefManager1Dim<T>::Delete(cstring name)
{
	T* item = GetItemByName(name);
	if (item != nullptr)
	{
		auto it = objects.find(name);

		this->DeleteAction(name);
		delete item;
		objects.erase(name);
		item = nullptr;
	}

}

template <class T>
void RefManager1Dim<T>::Delete(T* item)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (it->second == item)
		{
			this->Delete(it->first);
			break;
		}
	}
}

template <class T>
T* RefManager1Dim<T>::GetItemByName(cstring name) const
{
	auto name_item = objects.find(name);
	if (name_item != objects.end())
	{
		return name_item->second;
	}

	return nullptr;
}

template <class T>
std::string RefManager1Dim<T>::GetNameByItem(T* item) const
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (it->second == item)
		{
			return it->first;
		}
	}

	return "";
}

// CLog

template <class T>
void RefManager1Dim<T>::PrintAllInfo()
{
	uint32 refsCnt = 0;
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		refsCnt += it->second->GetRefsCount();
		Log::Info("Item (%d) [%s]", it->second->GetRefsCount(), it->first.c_str());
	}

	Log::Info("Item's count [%d], items refs [%d]", objects.size(), refsCnt);
}

template<class T>
inline T* RefManager1Dim<T>::CreateAction(cstring name)
{
	return NULL;
}

template<class T>
inline bool RefManager1Dim<T>::DeleteAction(cstring name)
{
	return false;
}
