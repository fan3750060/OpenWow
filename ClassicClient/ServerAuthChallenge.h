#pragma once

#include "AuthCodes.h"

struct ServerAuthChallenge
{
public:
	ServerAuthChallenge(ByteBuffer& reader)
	{
		command = eAuthCmd::AUTH_LOGON_CHALLENGE;
		reader.readBytes(&unk2, 1);
		reader.readBytes(&error, 1);

		if (error != AuthResult::WOW_SUCCESS)
		{
			Log::Error("Server challege not accept (0x%X)", error);
			return;
		}

		reader.readBytes(B, 32);

		reader.readBytes(&gLen);
		reader.readBytes(g);

		reader.readBytes(&nLen);
		reader.readBytes(N, nLen);

		reader.readBytes(s, 32);
		reader.readBytes(unk3, 16);

		reader.readBytes(&securityFlags);
	}

public:
	eAuthCmd	command;
	uint8		unk2;
	AuthResult	error;
	uint8		B[32];
	uint8		gLen;
	uint8		g[64];
	uint8		nLen;
	uint8		N[64];
	uint8		s[32];
	uint8		unk3[16];
	uint8		securityFlags;
};
