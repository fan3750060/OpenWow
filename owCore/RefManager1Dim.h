#pragma once

template <class T>
class CRefManager1Dim
{
public:
	CRefManager1Dim();
	~CRefManager1Dim();

	shared_ptr<T> Add(cstring name);

	bool Exists(cstring name) const;

	void Delete(cstring name);
	void Delete(shared_ptr<T> item);

	shared_ptr<T> GetItemByName(cstring name) const;
	string GetNameByItem(shared_ptr<T> item) const;

	// Console
	void PrintAllInfo();

protected:
	virtual shared_ptr<T> CreateAction(cstring name);
	virtual bool DeleteAction(cstring name);

public:
	std::map<string, shared_ptr<T>> objects; // name - item
};

#include "RefManager1Dim.inl"