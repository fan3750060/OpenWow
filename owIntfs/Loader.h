#pragma once

// Forward BEGIN
__interface IManager;
// Forward END

__interface ILoadable
{
	bool Load();
	bool Delete();

	bool IsLoaded() const;
	void SetLoaded();
};

__interface
	__declspec(uuid("18A045EF-D291-45E5-8F61-E223C4AFEF83"))
	ILoader : public IManager
{
	void AddToLoadQueue(ILoadable* _item);
	void LoadAll();

	void AddToDeleteQueue(ILoadable* _item);
	void DeleteAll();
};