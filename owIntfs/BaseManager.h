#pragma once

__interface IManager
{

};

__interface IBaseManager
{
	void RegisterManager(IManager* _manager);
	void UnregisterManager(IManager* _manager);

	IManager* GetManager();
};