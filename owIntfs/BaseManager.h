#pragma once

struct
	__declspec(novtable, uuid("BDC2768B-055D-42EA-ABE3-CF17CD21178D"))
	IManager
{
	virtual ~IManager() = 0 {};
};

template<class T>
struct IRefManager : public IManager
{	
	virtual ~IRefManager() {};

	virtual std::shared_ptr<T> Add(cstring name) = 0;
	virtual bool Exists(cstring name) const = 0;
	virtual void Delete(cstring name) = 0;
	virtual void Delete(std::shared_ptr<T> item) = 0;
};



// FORWARD BEGIN
class CFontMesh;
// FORWARD END

struct
	__declspec(novtable, uuid("1427E242-CCB8-4AEC-ABC8-17DE58A96B05"))
	IFontsManager : public IRefManager<CFontMesh>
{
	virtual ~IFontsManager() {};

	virtual std::shared_ptr<CFontMesh> GetMainFont() const = 0;
};



// FORWARD BEGIN
class WMO;
// FORWARD END

struct 
	__declspec(novtable, uuid("42D47100-B825-47F1-BE2F-6F7C78443884"))
	IWMOManager : public IRefManager<WMO>
{
	virtual ~IWMOManager() {};
};


// FORWARD BEGIN
class M2;
// FORWARD END

struct
	__declspec(novtable, uuid("B14D922C-BE9E-44CA-9448-5400E3CB573A"))
	IM2Manager : public IRefManager<M2>
{
	virtual ~IM2Manager() {};
};

//--

enum SBaseManagerPhases : uint8
{
	Phase_Input = 0,
	Phase_Update,
	Phase_Pre3D,
	Phase_3D,
	Phase_Post3D,
	Phase_UI,
	Phase_NONE
};

struct 
	__declspec(novtable, uuid("BB9FD479-C7AD-4F57-837B-E299A04AF171"))
	IBaseManager
{
	virtual void RegisterManager(GUID _type, std::shared_ptr<IManager> _manager) = 0;
	virtual void UnregisterManager(GUID _type) = 0;

	virtual std::shared_ptr<IManager> GetManager(GUID _type) = 0;

	virtual void SetPhase(SBaseManagerPhases _phase) = 0;
	virtual SBaseManagerPhases GetPhase() = 0;
};