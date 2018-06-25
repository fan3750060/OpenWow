#pragma once

#include "AuthCodes.h"

class ServerAuthProof
{
public:
	ServerAuthProof(ByteBuffer& reader)
	{
		proto.cmd = AUTH_LOGON_PROOF;
		reader.readBytes(&proto.error);
		reader.readBytes(proto.M2, 20);
		reader.readBytes(&proto.AccountFlags, 4);
		reader.readBytes(&proto.SurveyId, 4);
		reader.readBytes(&proto.unk3, 2);
	}

	sAuthLogonProof_S proto;
};