#include "stdafx.h"

// Include
#include "AuthWorldController.h"

// General
#include "AuthSocket.h"

CAuthSocket::CAuthSocket(CAuthWorldController* _world) :
	m_World(_world)
{
	socketBase = new CSocketBase(m_World->getHost(), m_World->getPort());
}

CAuthSocket::~CAuthSocket()
{
	delete socketBase;
}

//--

void CAuthSocket::SendData(const IByteBufferOutput& _bb)
{
	socketBase->SendData(_bb.getData(), _bb.getSize());
}

void CAuthSocket::SendData(const IByteBuffer& _bb)
{
	socketBase->SendData(_bb.getData(), _bb.getSize());
}

void CAuthSocket::SendData(const uint8* _data, uint32 _count)
{
	socketBase->SendData(_data, _count);
}

void CAuthSocket::InitHandlers()
{
	m_Handlers[AUTH_LOGON_CHALLENGE] = &CAuthSocket::S_LoginChallenge;
	m_Handlers[AUTH_LOGON_PROOF] = &CAuthSocket::S_LoginProof;
	m_Handlers[REALM_LIST] = &CAuthSocket::S_Realmlist;

	socketBase->setOnReceiveCallback(FUNCTION_CLASS_WA_Builder(CAuthSocket, this, OnDataReceive, ByteBuffer));
}

void CAuthSocket::OnDataReceive(ByteBuffer _buf)
{
	eAuthCmd currHandler;
	_buf.readBytes(&currHandler);
	Log::Info("Handler [0x%X]", currHandler);

	ProcessHandler(currHandler, _buf);
}

void CAuthSocket::ProcessHandler(eAuthCmd _handler, ByteBuffer _buffer)
{
	std::unordered_map<eAuthCmd, HandlerFunc>::iterator handler = m_Handlers.find(_handler);
	if (handler != m_Handlers.end())
	{
		(*this.*handler->second)(_buffer);
	}
	else
	{
		Log::Error("Handler [0x%X] not found!", _handler);
	}
}

//-- Client to server
#include "AuthChallenge_C.h"

void CAuthSocket::C_SendLogonChallenge()
{
	AuthChallenge_C challenge(m_World->getUsername(), socketBase->getMyInet());
	challenge.Send(this);
}

//-- Server to client

#include "AuthChallenge_S.h"
#include "AuthProof_C.h"

bool CAuthSocket::S_LoginChallenge(ByteBuffer& _buff)
{
	AuthChallenge_S challenge(_buff);

	char buff[256];
	sprintf(buff, "%s:%s", m_World->getUsername().c_str(), m_World->getPassword().c_str());
	string NameAndPass = Utils::ToUpper(buff);

	BigNumber A, u, x;
	BigNumber k(3);

	uint8 m2[512];

#pragma region Receive and initialize
	BigNumber B, g, N, s;
	B.SetBinary(challenge.B, 32);					// server public key
	g.SetBinary(challenge.g, challenge.gLen);
	N.SetBinary(challenge.N, challenge.nLen);		// modulus
	s.SetBinary(challenge.s, 32);

	//Log::Info("---====== Received from server: ======---");
	//Log::Info("B=%s", B.AsHexStr().c_str());
	//Log::Info("g=%s", B.AsHexStr().c_str());
	//Log::Info("N=%s", N.AsHexStr().c_str());
	//Log::Info("s=%s", s.AsHexStr().c_str());
#pragma endregion

#pragma region Hash password
	SHA1Hash PasswordHash;
	PasswordHash.Initialize();
	PasswordHash.UpdateData((const uint8*)NameAndPass.c_str(), NameAndPass.size());
	PasswordHash.Finalize();

	// x = H(s, p) (пользователь вводит пароль) salt + password
	SHA1Hash xSHA;
	xSHA.Initialize();
	xSHA.UpdateData(challenge.s, 32); // TODO sizes
	xSHA.UpdateData(PasswordHash.GetDigest(), SHA_DIGEST_LENGTH);
	xSHA.Finalize();
	x.SetBinary(xSHA.GetDigest(), SHA_DIGEST_LENGTH);

	//Log::Info("---====== shared password hash ======---");
	//Log::Info("p=%s", PasswordHash.toString().c_str());
	//Log::Info("x=%s", x.AsHexStr().c_str());
#pragma endregion

#pragma region Create random key pair
	BigNumber a;
	a.SetRand(19 * 8);

	A = g.ModExp(a, N);

	//Log::Info("---====== Send data to server: ======---");
	//Log::Info("a=%s", a.AsHexStr().c_str());
	//Log::Info("A=%s", A.AsHexStr().c_str());
#pragma endregion

#pragma region Compute session key
	SHA1Hash uSHA;
	uSHA.Initialize();
	uSHA.UpdateBigNumbers(&A, &B, nullptr);
	uSHA.Finalize();
	u.SetBinary(uSHA.GetDigest(), SHA_DIGEST_LENGTH);

	// compute session key
	// TODO: session key computation fails for some reason all variables match exactly to the server side, but S is different
	BigNumber S = (B - k * g.ModExp(x, N)).ModExp(a + u * x, N);

	uint8 keyData[40];

	uint8 t[32];
	memcpy(t, S.AsByteArray(32).get(), 32);

	// take every even indices byte, hash, store in even indices
	uint8 t1[16];
	for (int i = 0; i < 16; ++i) t1[i] = t[i * 2];

	SHA1Hash sha;
	sha.Initialize();
	sha.UpdateData(t1, 16);
	sha.Finalize();
	for (uint32 i = 0; i < SHA_DIGEST_LENGTH; ++i) keyData[i * 2] = sha.GetDigest()[i];

	// do the same for odd indices
	for (uint32 i = 0; i < 16; ++i) t1[i] = t[i * 2 + 1];

	sha.Initialize();
	sha.UpdateData(t1, 16);
	sha.Finalize();
	for (uint32 i = 0; i < SHA_DIGEST_LENGTH; ++i)	keyData[i * 2 + 1] = sha.GetDigest()[i];

	Key.SetBinary(keyData, SHA_DIGEST_LENGTH * 2);

	//Log::Info("---====== Compute session key ======---");
	//Log::Info("K=%s", Key.AsHexStr().c_str());
#pragma endregion

#pragma region Generate crypto proof
	// H(N)
	sha.Initialize();
	sha.UpdateBigNumbers(&N, nullptr);
	sha.Finalize();
	uint8 HN[SHA_DIGEST_LENGTH];
	memcpy(HN, sha.GetDigest(), SHA_DIGEST_LENGTH);

	// H(g)
	sha.Initialize();
	sha.UpdateBigNumbers(&g, nullptr);
	sha.Finalize();
	uint8 HG[SHA_DIGEST_LENGTH];
	memcpy(HG, sha.GetDigest(), SHA_DIGEST_LENGTH);

	// H(N) xor H(g)
	uint8 HNxorHG[SHA_DIGEST_LENGTH];
	for (uint32 i = 0; i < SHA_DIGEST_LENGTH; ++i)	HNxorHG[i] = HN[i] ^ HG[i];

	// H(I)
	sha.Initialize();
	sha.UpdateData((const uint8*)m_World->getUsername().c_str(), m_World->getUsername().size());
	sha.Finalize();

	// M
	SHA1Hash MClient;
	MClient.Initialize();
	MClient.UpdateData(HNxorHG, SHA_DIGEST_LENGTH);
	MClient.UpdateData(sha.GetDigest(), SHA_DIGEST_LENGTH);
	MClient.UpdateBigNumbers(&s, &A, &B, &Key, nullptr);
	MClient.Finalize();

	//Log::Info("---====== Send proof to server: ======---");
	//Log::Info("MC=%s", MClient.toString().c_str());
#pragma endregion

#pragma region Send proof
	AuthProof_C authProof(A.AsByteArray(32).get(), MClient.GetDigest());
	authProof.Send(this);
#pragma endregion

	// expected proof for server
	MServer.Initialize();
	MServer.UpdateBigNumbers(&A, nullptr);
	MServer.UpdateData(MClient.GetDigest(), SHA_DIGEST_LENGTH);
	MServer.UpdateBigNumbers(&Key, nullptr);
	MServer.Finalize();

	return true;
}

#include "AuthProof_S.h"

bool CAuthSocket::S_LoginProof(ByteBuffer& _buff)
{
	AuthProof_S proof(_buff);

	switch (proof.error)
	{
	case AuthResult::WOW_FAIL_VERSION_UPDATE:
	{
		Log::Error("Client update requested");
	}
	break;

	case AuthResult::WOW_FAIL_UNKNOWN_ACCOUNT:
	case AuthResult::WOW_FAIL_INCORRECT_PASSWORD:
	{
		Log::Error("Wrong password or invalid account or authentication error");
	}
	break;

	case AuthResult::WOW_FAIL_VERSION_INVALID:
	{
		Log::Error("Wrong build number");

	}
	break;

	default:
	{
		if (proof.error != AuthResult::WOW_SUCCESS)
		{
			Log::Error("Unkown error [%X]", proof.error);
		}
	}
	break;

	}

	// Send another logon challenge
	if (proof.error != AuthResult::WOW_SUCCESS)
	{
		Sleep(1000);
		C_SendLogonChallenge();
		return false;
	}

	// Check server M
	bool result = true;
	for (uint32 i = 0; i < SHA_DIGEST_LENGTH; i++)
	{
		if (proof.M2[i] != MServer.GetDigest()[i])
		{
			result = false;
			break;
		}
	}
	if (result)
	{
		Log::Green("All ok! Server proof equal client calculated server proof!");
	}
	else
	{
		Log::Error("Server 'M' mismatch!");
	}

	Log::Green("Successfully logined!!!");


	ByteBufferOutput bb2;
	bb2.Write((uint8)REALM_LIST);
	bb2.Write((uint32)0);
	SendData(bb2.getData(), bb2.getSize());

	return true;
}

bool CAuthSocket::S_Realmlist(ByteBuffer& _buff)
{
	uint16 bufferSize;
	_buff.readBytes(&bufferSize, 2);

	uint32 realmlistBufferSize;
	_buff.readBytes(&realmlistBufferSize, 4);

	uint16 count;
	_buff.readBytes(&count, 2);
	Log::Green("S_Realmlist: Count [%d]", count);

	for (uint32 i = 0; i < count; i++)
	{
		RealmInfo rinfo(_buff);	
		m_World->AddRealm(rinfo);
	}

	m_World->OnSuccessConnect(Key);

	return true;
}
