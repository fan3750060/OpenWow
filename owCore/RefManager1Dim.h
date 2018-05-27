#pragma once

#include "RefItem.h"

template <class T>
class RefManager1Dim
{
public:
	RefManager1Dim();
	~RefManager1Dim();

	T* Add(cstring name);

	bool Exists(cstring name) const;

	void Delete(cstring name);
	void Delete(T* item);

	T* GetItemByName(cstring name) const;
	string GetNameByItem(T* item) const;

	// Console
	void PrintAllInfo();

protected:
	virtual T* CreateAction(cstring name);
	virtual bool DeleteAction(cstring name);

public:
	std::map<string, T*> objects; // name - item
};

#include "RefManager1Dim.inl"