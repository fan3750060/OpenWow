#pragma once

// Forward BEGIN
__interface IManager;
// Forward END

__interface ISettingGroup
{
	void InitDefault();
};

__interface 
	__declspec(uuid("9FB1EEC3-16B7-4983-84C5-DDF47B203C6D"))
	ISettings : public IManager
{
	void RegisterBool(cstring _name, bool _defaultValue);
	bool& GetBool(cstring _name);

	void RegisterUint32(cstring _name, uint32 _defaultValue);
	uint32& GetUint32(cstring _name);

	void RegisterString(cstring _name, string _defaultValue);
	string& GetString(cstring _name);

	void RegisterGroup(GUID _guid, ISettingGroup* _group);
	ISettingGroup* GetGroup(GUID _guid);
};