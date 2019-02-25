#pragma once

#include "AuthCodes.h"

struct AuthProof_S
{
	AuthProof_S(CByteBuffer& reader)
	{
		cmd = AUTH_LOGON_PROOF;
		reader.readBytes(&error);
		if (error != AuthResult::WOW_SUCCESS)
		{
			uint16 t;
			reader.readBytes(&t, 2);
			return;
		}
		reader.readBytes(M2, 20);
		reader.readBytes(&AccountFlags, 4);
		reader.readBytes(&SurveyId, 4);
		reader.readBytes(&unk3, 2);
	}

	uint8   cmd;
	uint8   error;
	uint8   M2[20];
	uint32  AccountFlags;
	uint32  SurveyId;
	uint16  unk3;
};