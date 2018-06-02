#pragma once

// Forward
class R_Texture;
class Font;
class WMO;
class MDX;

__interface 
	__declspec(uuid("BDC2768B-055D-42EA-ABE3-CF17CD21178D"))
	IManager
{};

template<class T>
__interface IRefManager : public IManager
{	
	T* Add(cstring name);
	bool Exists(cstring name) const;
	void Delete(cstring name);
	void Delete(T* item);
};


__interface 
	__declspec(uuid("5C02C392-FD7A-4013-AAB7-43C8E81602AB"))
	ITexturesManager : public IRefManager<R_Texture>
{};


__interface 
	__declspec(uuid("1427E242-CCB8-4AEC-ABC8-17DE58A96B05"))
	IFontsManager : public IRefManager<Font>
{};


__interface 
	__declspec(uuid("42D47100-B825-47F1-BE2F-6F7C78443884"))
	IWMOManager : public IRefManager<WMO>
{};

__interface 
	__declspec(uuid("B14D922C-BE9E-44CA-9448-5400E3CB573A"))
	IMDXManager : public IRefManager<MDX>
{};

//--

__interface 
	__declspec(uuid("BB9FD479-C7AD-4F57-837B-E299A04AF171"))
	IBaseManager
{
	void RegisterManager(GUID _type, IManager* _manager);
	void UnregisterManager(GUID _type);

	IManager* GetManager(GUID _type);
};