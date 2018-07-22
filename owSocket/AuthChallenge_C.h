#pragma once

struct AuthChallenge_C : public ISendable
{
	AuthChallenge_C(string _username, uint32 _ip) :
		cmd(AUTH_LOGON_CHALLENGE),
		username(_username),
		ip(_ip)
	{
		version1 = 3;
		version2 = 3;
		version3 = 5;
		build = 12340;
	}

	void Send(ISocket* _socket) override
	{
		ByteBuffer bb;
		bb.Write(cmd);
		bb.Write((uint8)6);
		bb.Write((uint8)(username.size() + 30));
		bb.Write((uint8)0);

		bb.Write(gamename, 4);
		bb.Write(version1);
		bb.Write(version2);
		bb.Write(version3);
		bb.Write(build);
		bb.Write("68x", 4);
		bb.Write("niW", 4);
		bb.Write("URur", 4);
		bb.Write((uint32)180);
		bb.Write(ip);
		bb.Write((uint8)username.size());
		bb.Write(username);

		_socket->SendData(bb);
	}

	//--

	uint8   cmd;
	uint16  size;
	uint8   gamename[4];
	uint8   version1;
	uint8   version2;
	uint8   version3;
	uint16  build;
	uint32  ip;
	string	username;
};