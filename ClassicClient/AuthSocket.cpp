#include "stdafx.h"

// General
#include "AuthSocket.h"

CAuthSocket::CAuthSocket(const char* _host, const char * _port) :
	ConnectSocket(INVALID_SOCKET)
{
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		Log::Error("WSAStartup failed with error: %d", iResult);
		fail1();
	}

	ADDRINFOA hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	PADDRINFOA result = NULL;
	iResult = GetAddrInfoA(_host, _port, &hints, &result);
	if (iResult != 0)
	{
		Log::Error("getaddrinfo failed with error: %d", iResult);
		WSACleanup();
		fail1();
	}

	// Attempt to connect to an address until one succeeds
	for (PADDRINFOA ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET)
		{
			Log::Error("socket failed with error: %ld", WSAGetLastError());
			WSACleanup();
			fail1();
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}

		break;
	}

	FreeAddrInfoA(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		Log::Error("Unable to connect to server!");
		WSACleanup();
		fail1();
	}
}

CAuthSocket::~CAuthSocket()
{
	// shutdown the connection since no more data will be sent
	int iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		Log::Error("shutdown failed with error: %d", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		fail1();
	}

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
}

//--

void CAuthSocket::SendData(const uint8* _data, uint32 _count)
{
	int32 sendResult = send(ConnectSocket, (const char*)_data, _count, 0);
	if (sendResult == SOCKET_ERROR)
	{
		Log::Error("SendData failed with error: %d", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		fail1();
	}

	Log::Warn("CAuthSocket::SendData [%d] bytes sended.", sendResult);
}

void CAuthSocket::ReceiveLoop()
{
	const uint32 C_ReceiveBufferSize = 512;
	int32 result = 0;
	do
	{
		char receiveBuffer[C_ReceiveBufferSize];
		int32 receiveBufferSize = C_ReceiveBufferSize;
		result = recv(ConnectSocket, receiveBuffer, receiveBufferSize, 0);

		if (result > 0)
		{
			Log::Warn("Bytes received: [%d]", result);
			Log::Warn("First is [%X]", receiveBuffer[0]);

			m_ByteBuffer.Clear();
			m_ByteBuffer.Init((uint8*)receiveBuffer, result);

			eAuthCmd currHandler;
			m_ByteBuffer.readBytes(&currHandler);

			ProcessHandler(currHandler);
		}
		else if (result == 0)
		{
			Log::Error("Connection closed");
		}
		else
		{
			Log::Error("recv failed with error: [%d]", WSAGetLastError());
		}

	} while (result > 0);
}

#include "ServerAuthChallenge.h"
#include "ServerAuthProof.h"

void CAuthSocket::SendLogonChallenge()
{
	string username = DEFAULT_USERNAME;

	uint8 version[] = { 3, 3, 5 };
	uint16 build = 12340;

	// ClientAuthChallenge
	ByteBufferOutput bb;
	bb.Write((uint8)AUTH_LOGON_CHALLENGE);
	bb.Write((uint8)6);
	bb.Write((uint8)(username.size() + 30));
	bb.Write((uint8)0);
	bb.Write("WoW", 4);
	bb.Write(version, 3);
	bb.Write(build);
	bb.Write("68x", 4);
	bb.Write("niW", 4);
	bb.Write("URur", 4);
	bb.Write((uint32)180);
	bb.Write((uint32)getMyInet());
	bb.Write((uint8)username.size());
	bb.Write(username);

	SendData(bb.getData(), bb.getSize());
}

void CAuthSocket::ProcessHandler(eAuthCmd handler)
{
	switch (handler)
	{
	case AUTH_LOGON_CHALLENGE:
	{
		
	}
	break;

	case AUTH_LOGON_PROOF:
	{
		ServerAuthProof proof(m_ByteBuffer);

		switch (proof.proto.error)
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
			if (proof.proto.error != AuthResult::WOW_SUCCESS)
			{
				Log::Error("Unkown error [%X]", proof.proto.error);
			}
		}
		break;

		}

		if (proof.proto.error != AuthResult::WOW_SUCCESS)
		{
			SendLogonChallenge();
			break;
		}

		bool result = true;
		for (uint32 i = 0; i < SHA_DIGEST_LENGTH; i++)
		{
			if (proof.proto.M2[i] != MServer.GetDigest()[i])
			{
				result = false;
				break;
			}
		}

		if (result)
		{
			Log::Green("All ok! Server proof equal client calculated server proof!");
		}
		Log::Warn("Successfully logined!!!");


		ByteBufferOutput bb2;
		bb2.Write((uint8)REALM_LIST);
		bb2.Write((uint8)0);
		SendData(bb2.getData(), bb2.getSize());
	}
	break;

	case REALM_LIST:
	{

	}
	break;

	default:
	{
		Log::Error("Unknown byte [%d]", handler);
	}
	break;
	}

}

bool CAuthSocket::H_LoginChallenge(ByteBuffer& _buff)
{
	ServerAuthChallenge challenge(_buff);

	char buff[256];
	sprintf(buff, "%s:%s", DEFAULT_USERNAME, DEFAULT_PASS);
	string NameAndPass = Utils::ToUpper(buff);

	BigNumber  A, u, x;
	BigNumber k(3);

	BigNumber Key;
	uint8 m2[512];

#pragma region Receive and initialize
	BigNumber B, g, N, s;
	B.SetBinary(challenge.B, 32);					// server public key
	g.SetBinary(challenge.g, challenge.gLen);
	N.SetBinary(challenge.N, challenge.nLen);		// modulus
	s.SetBinary(challenge.s, 32);

	Log::Info("---====== Received from server: ======---");
	Log::Info("B=%s", B.AsHexStr().c_str());
	Log::Info("g=%s", B.AsHexStr().c_str());
	Log::Info("N=%s", N.AsHexStr().c_str());
	Log::Info("s=%s", s.AsHexStr().c_str());
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

	Log::Info("---====== shared password hash ======---");
	Log::Info("p=%s", PasswordHash.toString().c_str());
	Log::Info("x=%s", x.AsHexStr().c_str());
#pragma endregion

#pragma region Create random key pair
	BigNumber a;
	a.SetRand(19 * 8);

	A = g.ModExp(a, N);

	Log::Info("---====== Send data to server: ======---");
	Log::Info("a=%s", a.AsHexStr().c_str());
	Log::Info("A=%s", A.AsHexStr().c_str());
#pragma endregion

#pragma region Compute session key
	SHA1Hash uSHA;
	uSHA.Initialize();
	uSHA.UpdateBigNumbers(&A, &B);
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

	Log::Info("---====== Compute session key ======---");
	Log::Info("K=%s", Key.AsHexStr().c_str());
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
	sha.UpdateData((const uint8*)DEFAULT_USERNAME, strlen(DEFAULT_USERNAME));
	sha.Finalize();

	// M
	SHA1Hash MClient;
	MClient.Initialize();
	MClient.UpdateData(HNxorHG, SHA_DIGEST_LENGTH);
	MClient.UpdateData(sha.GetDigest(), SHA_DIGEST_LENGTH);
	MClient.UpdateBigNumbers(&s, &A, &B, &Key, nullptr);
	MClient.Finalize();

	Log::Info("---====== Send proof to server: ======---");
	Log::Info("MC=%s", MClient.toString().c_str());
#pragma endregion

#pragma region Send proof
	uint8 crc[20];

	ByteBufferOutput bb2;
	bb2.Write((uint8)AUTH_LOGON_PROOF);
	bb2.Write(A.AsByteArray(32).get(), A.GetNumBytes());
	bb2.Write(MClient.GetDigest(), SHA_DIGEST_LENGTH);
	bb2.Write(crc, 20);
	bb2.Write((uint8)0);
	bb2.Write((uint8)0);
	assert1(bb2.getSize() == sizeof(AUTH_LOGON_PROOF_C));

	SendData(bb2.getData(), bb2.getSize());

#pragma endregion

	// expected proof for server
	MServer.Initialize();
	MServer.UpdateBigNumbers(&A, nullptr);
	MServer.UpdateData(MClient.GetDigest(), SHA_DIGEST_LENGTH);
	MServer.UpdateBigNumbers(&Key, nullptr);
	MServer.Finalize();

	return true;
}
