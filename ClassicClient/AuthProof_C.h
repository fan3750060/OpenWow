#pragma once

struct AuthProof_C
{
	AuthProof_C(uint8* _A, uint8* _MClient)
	{
		cmd = AUTH_LOGON_PROOF;
		memcpy(A, _A, 32);
		memcpy(M1, _MClient, SHA_DIGEST_LENGTH);
	}

	void Send(ISocket* _socket)
	{
		uint8 crc[20];

		ByteBufferOutput bb;
		bb.Write(cmd);
		bb.Write(A, 32);
		bb.Write(M1, SHA_DIGEST_LENGTH);
		bb.Write(crc, 20);
		bb.Write((uint8)0);
		bb.Write((uint8)0);
		_socket->SendData(bb);
	}

	uint8   cmd;
	uint8   A[32];
	uint8   M1[SHA_DIGEST_LENGTH];
	uint8   crc_hash[20];
	uint8   number_of_keys;
	uint8   securityFlags;
};