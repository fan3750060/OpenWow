#pragma once

#include "AuthCodes.h"

struct AuthChallenge_S
{
	AuthChallenge_S(CByteBuffer& reader)
	{
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
