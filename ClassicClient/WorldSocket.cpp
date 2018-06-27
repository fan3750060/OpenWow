#include "stdafx.h"

// Include
#include "AuthWorldController.h"

// General
#include "WorldSocket.h"

CWorldSocket::CWorldSocket(CAuthWorldController* _world, string _host, string _port, string _username) :
	m_World(_world),
	m_Host(_host),
	m_Port(_port),
	m_Username(_username)
{
	socketBase = new CSocketBase(m_Host, m_Port);
}

CWorldSocket::~CWorldSocket()
{
	delete socketBase;
}

//--

void CWorldSocket::SendData(Opcodes _opcode)
{
	Opcodes command = (Opcodes)((&reinterpret_cast<uint8&>(_opcode))[1] << 8 | (&reinterpret_cast<uint8&>(_opcode))[0]);

	ByteBufferOutput finalBuffer;
	finalBuffer.Write((uint8)0);
	finalBuffer.Write((uint8)4);
	finalBuffer.Write((uint32)command);
	SendData(finalBuffer.getData(), finalBuffer.getSize());
}

void CWorldSocket::SendData(Opcodes _opcode, ByteBufferOutput& _bb)
{
	uint16 size0 = _bb.getSize() + 4 /* HEADER */;
	uint16 sizeFinal = ((&reinterpret_cast<uint8&>(size0))[0] << 8 | (&reinterpret_cast<uint8&>(size0))[1]);
	Opcodes command = (Opcodes)((&reinterpret_cast<uint8&>(_opcode))[1] << 8 | (&reinterpret_cast<uint8&>(_opcode))[0]);

	ByteBufferOutput finalBuffer;
	finalBuffer.Write(sizeFinal);
	finalBuffer.Write((uint32)command);
	finalBuffer.Write(_bb.getData(), _bb.getSize());

	SendData(finalBuffer.getData(), finalBuffer.getSize());
}

void CWorldSocket::SendData(const uint8* _data, uint32 _count)
{
	uint8 data2[512];
	memcpy(data2, _data, _count);

	cryptUtils.EncryptSend(data2, 6 /* size + header*/);

	socketBase->SendData(data2, _count);
}

#include "AuthChallenge_S.h"
#include "AuthProof_S.h"

void CWorldSocket::InitHandlers()
{
	m_Handlers[SMSG_AUTH_CHALLENGE] = &CWorldSocket::S_AuthChallenge;
	m_Handlers[SMSG_AUTH_RESPONSE] = &CWorldSocket::S_AuthResponse;

	socketBase->setOnReceiveCallback(FUNCTION_CLASS_WA_Builder(CWorldSocket, this, OnDataReceive, ByteBuffer));
}

void CWorldSocket::OnDataReceive(ByteBuffer _buf)
{
	uint32 currReader = 0;
	while (!_buf.isEof())
	{
		uint8* data = _buf.getDataFromCurrentEx();
		uint32 size = 0;
		uint16 command = 0;

		// 1. Decrypt size
		cryptUtils.DecryptRecv(data + 0, 1);
		//uint8 firstByte = data[0];
		//assert1((firstByte & 0x80) == 0)

		// 2. Decrypt other size
		cryptUtils.DecryptRecv(data + 1, 3);
		size	= (data[0] << 8 | data[1]);
		command = (data[3] << 8 | data[2]);
		Log::Info("CWorldSocket: Size [%d], Command [%d]", size - 2, size, command);

		// DEBUG
		if (size > _buf.getSize())
		{
			fail1();
		}
		
		_buf.seekRelative(4);

		InPacket packet;
		packet.opcode = (Opcodes)command;
		packet.size = size;
		packet.data = _buf;
		m_Packets.push(packet);

		_buf.seekRelative(size - 2);
	}
	_buf.Clear();

	//--

	while (!m_Packets.empty())
	{
		InPacket& pack = m_Packets.front();
		ProcessHandler(pack.opcode, pack.data);
		m_Packets.pop();
	}
}

void CWorldSocket::ProcessHandler(Opcodes _handler, ByteBuffer _buffer)
{
	std::unordered_map<Opcodes, HandlerFunc>::iterator handler = m_Handlers.find(_handler);
	if (handler != m_Handlers.end())
	{
		(*this.*handler->second)(_buffer);
	}
	else
	{
		Log::Error("Opcodes [0x%X] not found!", _handler);
	}
}

bool CWorldSocket::S_AuthChallenge(ByteBuffer& _buff)
{
	uint32 one;
	_buff.readBytes(&one, 4);

	uint32 seed; 
	_buff.readBytes(&seed, 4);

	_buff.seek(32);
	
	BigNumber ourSeed;
	ourSeed.SetRand(4 * 8);

	string upperUsername = Utils::ToUpper(m_Username);
	uint8 zeroBytes[] = { 0, 0, 0, 0 };

	SHA1Hash uSHA;
	uSHA.Initialize();
	uSHA.UpdateData((const uint8*)upperUsername.c_str(), upperUsername.size());
	uSHA.UpdateData(zeroBytes, 4);
	uSHA.UpdateBigNumbers(&ourSeed, nullptr);
	uSHA.UpdateData(&reinterpret_cast<uint8&>(seed), 4);
	uSHA.UpdateBigNumbers(m_World->getKey(), nullptr);
	uSHA.Finalize();

	ByteBufferOutput bb;
	bb.Write((uint32)12345);
	bb.WriteDummy(4);
	bb.Write((const uint8*)upperUsername.c_str(), upperUsername.size() + 1);
	bb.WriteDummy(4);
	bb.Write(ourSeed.AsByteArray(4).get(), 4);
	bb.WriteDummy(20);
	bb.Write(uSHA.GetDigest(), SHA_DIGEST_LENGTH);
	bb.WriteDummy(4);

	SendData(CMSG_AUTH_SESSION, bb);

	// Start encription from here
	cryptUtils.Init(m_World->getKey());

	return true;
}

bool CWorldSocket::S_AuthResponse(ByteBuffer& _buff)
{
	enum CommandDetail : uint8
	{
		AuthSuccess = 0x0C,
		AuthQueue,
		AuthFailure
	};

	CommandDetail detail;
	_buff.readBytes(&detail);

	uint32 billingTimeRemaining;
	_buff.readBytes(&billingTimeRemaining, 4);

	uint8 billingFlags;
	_buff.readBytes(&billingFlags, 1);

	uint32 billingTimeRested;
	_buff.readBytes(&billingTimeRested, 4);

	uint8 expansion;
	_buff.readBytes(&expansion, 1);

	if (detail == CommandDetail::AuthSuccess)
	{
		SendData(CMSG_CHAR_ENUM);
	}
	else if (detail == CommandDetail::AuthSuccess)
	{
		Log::Warn("You in queue!!!");
	}
	else
	{
		Log::Warn("Auth failed");
	}

	return true;
}


/*[PacketHandler(WorldCommand.ServerAuthResponse)]
void HandleServerAuthResponse(InPacket packet)
{
	CommandDetail detail = (CommandDetail)packet.ReadByte();

	uint billingTimeRemaining = packet.ReadUInt32();
	byte billingFlags = packet.ReadByte();
	uint billingTimeRested = packet.ReadUInt32();
	byte expansion = packet.ReadByte();

	if (detail == CommandDetail.AuthSuccess)
	{
		OutPacket request = new OutPacket(WorldCommand.ClientEnumerateCharacters);
		Send(request);
	}
	else
	{
		Game.UI.LogLine(string.Format("Authentication succeeded, but received response {0}", detail));
		Game.UI.Exit();
	}
}

[PacketHandler(WorldCommand.ServerCharacterEnumeration)]
void HandleCharEnum(InPacket packet)
{
	byte count = packet.ReadByte();

	if (count == 0)
	{
		Game.UI.LogLine("No characters found!");
	}
	else
	{
		Character[] characters = new Character[count];
		for (byte i = 0; i < count; ++i)
			characters[i] = new Character(packet);

		Game.UI.PresentCharacterList(characters);
	}
}*/