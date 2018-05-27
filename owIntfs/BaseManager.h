#pragma once

typedef enum Managers
{
	MgrTextures = 0,
	MgrFonts,
	MgrWMO,
	MgrMDX,
	MgrFiles
};

__interface IManager
{};

template<class T>
__interface IRefManager : public IManager
{	
	T* Add(cstring name);

	bool Exists(cstring name) const;

	void Delete(cstring name);
	void Delete(T* item);
};

class R_Texture;
__interface ITexturesManager : public IRefManager<R_Texture>
{};

class WMO;
__interface IWMOManager : public IRefManager<WMO>
{};

class MDX;
__interface IMDXManager : public IRefManager<MDX>
{};

//--

__interface IBaseManager
{
	void RegisterManager(Managers _type, IManager* _manager);
	void UnregisterManager(IManager* _manager);

	IManager* GetManager(Managers _type);
};