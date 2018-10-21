#pragma once

class RealmInfo
{
public:
	enum RealmFlags : uint8
	{
		REALM_FLAG_NONE = 0x00,
		REALM_FLAG_VERSION_MISMATCH = 0x01,
		REALM_FLAG_OFFLINE = 0x02,
		REALM_FLAG_SPECIFYBUILD = 0x04,
		REALM_FLAG_UNK1 = 0x08,
		REALM_FLAG_UNK2 = 0x10,
		REALM_FLAG_RECOMMENDED = 0x20,
		REALM_FLAG_NEW = 0x40,
		REALM_FLAG_FULL = 0x80
	};

	enum RealmType : uint8
	{
		REALM_TYPE_NORMAL = 0,
		REALM_TYPE_PVP = 1,
		REALM_TYPE_NORMAL2 = 4,
		REALM_TYPE_RP = 6,
		REALM_TYPE_RPPVP = 8,

		MAX_CLIENT_REALM_TYPE = 14,

		REALM_TYPE_FFA_PVP = 16     // custom, free for all pvp mode like arena PvP in all zones except rest activated places and sanctuaries
	};

	struct RealmBuildInfo
	{
		uint8 MajorVersion;
		uint8 MinorVersion;
		uint8 BugfixVersion;
		uint16 Build;

		std::string toString()
		{
			return to_string(MajorVersion) + "." + to_string(MinorVersion) + "." + to_string(BugfixVersion) + " (" + to_string(Build) + ")";
		}
	};

public:
	RealmInfo(ByteBuffer& _buff)
	{
		_buff.readBytes(&Type);
		_buff.readBytes(&Locked);
		_buff.readBytes(&Flag);
		_buff.readString(&Name);
		_buff.readString(&Addres);
		_buff.readBytes(&Population, 4);
		_buff.readBytes(&CharactersCount);
		_buff.readBytes(&TimeZone);
		_buff.readBytes(&RealmID);
		if (Flag & RealmInfo::REALM_FLAG_SPECIFYBUILD)
		{
			_buff.readBytes(&BuildInfo, sizeof(RealmBuildInfo));
		}
		//_buff.readBytes(&unk0);
		//_buff.readBytes(&unk1);

		// Calculate ip and port

		int32 pos = Addres.find(':');
		_ASSERT(pos != -1);
		m_IPAddres = Addres.substr(0, pos);
		m_Port = Addres.substr(pos + 1);
	}

	cstring getIP() const { return m_IPAddres; }
	cstring getPort() const { return m_Port; }

	void print()
	{
		Log::Green("Realm '%s', type '%d', addres '%s'", Name.c_str(), Type, Addres.c_str());
	}

public:
	RealmType Type;
	uint8 Locked;
	RealmFlags Flag;
	std::string Name;
	std::string Addres;
	float Population;
	uint8 CharactersCount;
	uint8 TimeZone;
	uint8 RealmID;
	RealmBuildInfo BuildInfo;

private:
	std::string m_IPAddres;
	std::string m_Port;
};