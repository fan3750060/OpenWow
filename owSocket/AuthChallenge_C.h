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
		bb << cmd;
		bb << (uint8)6;
		bb << (uint8)(username.size() + 30);
		bb << (uint8)0;

		bb.Append(gamename, 4);
		bb << version1;
		bb << version2;
		bb << version3;
		bb << build;
		bb.Append((const uint8*)"68x", 4);
		bb.Append((const uint8*)"niW", 4);
		bb.Append((const uint8*)"URur", 4);
		bb << (uint32)180;
		bb << ip;
		bb << (uint8)username.size();
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