#pragma once

struct AuthChallenge_C : public ISendable
{
	AuthChallenge_C(std::string _username, uint32 _ip) 
        : username(_username)
        , ip(_ip)
	{}

	void Send(TcpSocket* _socket) override
	{
		CByteBuffer bb;
		bb << (uint8)AUTH_LOGON_CHALLENGE;
		bb << (uint8)6;
		bb << (uint8)(username.size() + 30);
		bb << (uint8)0;

		bb.Append(gamename, 4);
		bb << version1;
		bb << version2;
		bb << version3;
		bb << build;
		bb.Append((const uint8*)"68x", 4);   // x86
		bb.Append((const uint8*)"niW", 4);   // Win
		bb.Append((const uint8*)"URur", 4);  // ruRU
		bb << (uint32)180;
		bb << ip;
		bb << (uint8)username.size();
		bb.Write(username);

        _socket->SendBuf(reinterpret_cast<const char*>(bb.getData()), bb.getSize());
	}

	//--

	uint8   gamename[4] = {};
    const uint8   version1 = 1;
    const uint8   version2 = 12;
    const uint8   version3 = 1;
	const uint16  build = 5875;
	uint32  ip;
	std::string	username;
};