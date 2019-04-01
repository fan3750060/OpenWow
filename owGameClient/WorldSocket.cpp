#include "stdafx.h"

// Include
#include "Client.h"

// General
#include "WorldSocket.h"

// Additional
#include "OpcodesNames.h"
#include "HMACSHA1.h"

const Opcodes IgnoredOpcodes[] = 
{
	Opcodes::SMSG_SET_PROFICIENCY,
	Opcodes::SMSG_LOGIN_VERIFY_WORLD,
	Opcodes::SMSG_ACCOUNT_DATA_TIMES,
	Opcodes::SMSG_FEATURE_SYSTEM_STATUS,
	Opcodes::SMSG_BINDPOINTUPDATE,
	Opcodes::SMSG_INITIAL_SPELLS,
	Opcodes::SMSG_SEND_UNLEARN_SPELLS,
	Opcodes::SMSG_ACTION_BUTTONS,
	Opcodes::SMSG_INITIALIZE_FACTIONS,
	Opcodes::SMSG_LOGIN_SETTIMESPEED,
	Opcodes::SMSG_SET_FORCED_REACTIONS,
	Opcodes::SMSG_COMPRESSED_UPDATE_OBJECT,
	Opcodes::SMSG_MONSTER_MOVE,

	Opcodes::SMSG_EMOTE,
	Opcodes::SMSG_TIME_SYNC_REQ,
	Opcodes::SMSG_SPELL_START,
	Opcodes::SMSG_SPELL_GO
};

CWorldSocket::CWorldSocket(ISocketHandler& SocketHandler, std::shared_ptr<CWoWClient> WoWClient)
    : TcpSocket(SocketHandler)
	, m_WoWClient(WoWClient)
    , currPacket(nullptr)
{
    InitHandlers();
}

CWorldSocket::~CWorldSocket()
{

	Log::Info("[WorldSocket]: All threads stopped.");
}

//--

void CWorldSocket::SendData(Opcodes _opcode)
{
	Opcodes command = (Opcodes)((&reinterpret_cast<uint8&>(_opcode))[1] << 8 | (&reinterpret_cast<uint8&>(_opcode))[0]);

	CByteBuffer finalBuffer;
	finalBuffer << (uint8)0;
	finalBuffer << (uint8)4;
	finalBuffer << (uint32)command;

	SendData(finalBuffer.getData(), finalBuffer.getSize());
}
void CWorldSocket::SendData(Opcodes _opcode, CByteBuffer& _bb)
{
	assert1((_bb.getSize() + 4) < UINT16_MAX);

	uint16 size0 = _bb.getSize() + 4 /* HEADER */;
	uint16 sizeFinal = ((&reinterpret_cast<uint8&>(size0))[0] << 8 | (&reinterpret_cast<uint8&>(size0))[1]);
	Opcodes command = (Opcodes)((&reinterpret_cast<uint8&>(_opcode))[1] << 8 | (&reinterpret_cast<uint8&>(_opcode))[0]);

	CByteBuffer finalBuffer;
	finalBuffer << sizeFinal;
	finalBuffer << (uint32)command;
	finalBuffer << _bb;

	SendData(finalBuffer.getData(), finalBuffer.getSize());
}
void CWorldSocket::SendData(const uint8* _data, uint32 _count)
{
	assert1(_count < 4096);

	uint8 data2[4096];
	memcpy(data2, _data, _count);

    m_WoWCryptoUtils.EncryptSend(data2, 6 /* size (2) + header (4)*/);

	//socketBase->SendData(data2, _count);
}

void CWorldSocket::OnConnect()
{
}

void CWorldSocket::OnRawData(const char * buf, size_t len)
{
    CByteBuffer bb;
    bb.Allocate(len);
    bb.CopyData(reinterpret_cast<const uint8*>(buf), len);

    OnDataReceive(bb);
}


void CWorldSocket::InitHandlers()
{
	m_Handlers[SMSG_AUTH_CHALLENGE] = std::bind(&CWorldSocket::S_AuthChallenge, this, std::placeholders::_1);
	m_Handlers[SMSG_AUTH_RESPONSE] = std::bind(&CWorldSocket::S_AuthResponse, this, std::placeholders::_1);
	
	// Dummy
	m_Handlers[SMSG_SET_PROFICIENCY] = nullptr;
	m_Handlers[SMSG_LOGIN_VERIFY_WORLD] = nullptr;
	m_Handlers[SMSG_ACCOUNT_DATA_TIMES] = nullptr;
	m_Handlers[SMSG_FEATURE_SYSTEM_STATUS] = nullptr;
	m_Handlers[SMSG_BINDPOINTUPDATE] = nullptr;
	m_Handlers[SMSG_INITIAL_SPELLS] = nullptr;
	m_Handlers[SMSG_SEND_UNLEARN_SPELLS] = nullptr;
	m_Handlers[SMSG_ACTION_BUTTONS] = nullptr;
	m_Handlers[SMSG_INITIALIZE_FACTIONS] = nullptr;
	m_Handlers[SMSG_LOGIN_SETTIMESPEED] = nullptr;
	m_Handlers[SMSG_SET_FORCED_REACTIONS] = nullptr;
	m_Handlers[SMSG_COMPRESSED_UPDATE_OBJECT] = nullptr;
	m_Handlers[SMSG_MONSTER_MOVE] = nullptr;

	m_Handlers[Opcodes::SMSG_EMOTE] = nullptr;
	m_Handlers[Opcodes::SMSG_TIME_SYNC_REQ] = nullptr;
	m_Handlers[Opcodes::SMSG_SPELL_START] = nullptr;
	m_Handlers[Opcodes::SMSG_SPELL_GO] = nullptr;
}

void CWorldSocket::OnDataReceive(CByteBuffer _buf)
{
	if (currPacket != nullptr)
	{
		Packet2(_buf);
	}

	while (!_buf.isEof())
	{
		uint8* data = _buf.getDataFromCurrentEx();
		uint32 packetSize = 0;
		uint8 sizeCorrection = 0;
		uint16 command = 0;

		// 1. Decrypt size
        m_WoWCryptoUtils.DecryptRecv(data + 0, 1);
		uint8 firstByte = data[0];

		// 2. Decrypt other size
		if ((firstByte & 0x80) != 0)
		{
			sizeCorrection = 3;
            m_WoWCryptoUtils.DecryptRecv(data + 1, 1 + sizeCorrection);
			packetSize = (/*((data[0] & 0x7F) << 16) |*/ (data[1] << 8) | data[2]);
			command = ((data[4] << 8) | data[3]);
		}
		else
		{
			sizeCorrection = 2;
            m_WoWCryptoUtils.DecryptRecv(data + 1, 1 + sizeCorrection);
			packetSize = ((data[0] << 8) | data[1]);
			command = ((data[3] << 8) | data[2]);
		}

		//
		bool needMess = true;
		for (uint32 i = 0; i < OW_COUNT_ELEMENTS(IgnoredOpcodes); i++)
		{
			if (IgnoredOpcodes[i] == command)
			{
				needMess = false;
				break;
			}
		}

		// DEBUG
		assert1(command < Opcodes::COUNT);
		Log::Info("CWorldSocket: Command '%s' (0x%X) size=%d", OpcodesNames[command].c_str(), command, packetSize - sizeCorrection);

		_buf.seekRelative(sizeCorrection + sizeof(Opcodes));

		Packet1(command, packetSize - sizeCorrection);
		Packet2(_buf);
	}
}

void CWorldSocket::AddHandler(Opcodes _opcode, HandlerFuncitonType _func)
{
	assert1(_func != nullptr);
	m_Handlers.insert(std::make_pair(_opcode, _func));
}

void CWorldSocket::ProcessHandler(Opcodes _handler, CByteBuffer _buffer)
{
	std::unordered_map<Opcodes, HandlerFuncitonType>::iterator handler = m_Handlers.find(_handler);
	if (handler != m_Handlers.end())
	{
		if (handler->second != nullptr)
		{
			(handler->second).operator()(_buffer);
		}
	}
}

//--

void CWorldSocket::Packet1(uint16 _command, uint32 _size)
{
	CByteBuffer buff;

	currPacket = new InPacket();
	currPacket->dataSize = _size;
	currPacket->opcode = (Opcodes)_command;
}

void CWorldSocket::Packet2(CByteBuffer& _buf)
{
	uint32 buffRead = currPacket->dataSize - currPacket->data.getSize();
	uint32 buffToEnd = _buf.getSize() - _buf.getPos();
	if (buffRead > buffToEnd)
	{
		buffRead = buffToEnd;
	}
	
	// Fill data
	assert1(_buf.getPos() + buffRead <= _buf.getSize());
	currPacket->data.Append(_buf.getDataFromCurrent(), buffRead);
	_buf.seekRelative(buffRead);
	Log::Info("Packet[%s] readed '%d' of '%d'.", OpcodesNames[currPacket->opcode].c_str(), currPacket->data.getSize(), currPacket->dataSize);

	// Final
	if (currPacket->data.getSize() == currPacket->dataSize)
	{
		ProcessHandler(currPacket->opcode, currPacket->data);

		SafeDelete(currPacket);
	}
}

//--

void CWorldSocket::S_AuthChallenge(CByteBuffer& _buff)
{
    std::shared_ptr<CWoWClient> WoWClient = m_WoWClient.lock();
    _ASSERT(WoWClient);

	uint32 seed; 
	_buff.readBytes(&seed, 4);
	
	BigNumber ourSeed;
	ourSeed.SetRand(4 * 8);

	std::string upperUsername = Utils::ToUpper(WoWClient->getUsername());
	uint8 zeroBytes[] = { 0, 0, 0, 0 };

	SHA1Hash uSHA;
	uSHA.Initialize();
	uSHA.UpdateData((const uint8*)upperUsername.c_str(), upperUsername.size());
	uSHA.UpdateData(zeroBytes, 4);
	uSHA.UpdateBigNumbers(&ourSeed, nullptr);
	uSHA.UpdateData(&reinterpret_cast<uint8&>(seed), 4);
	uSHA.UpdateBigNumbers(WoWClient->getKey(), nullptr);
	uSHA.Finalize();

	//------------------
	CByteBuffer bb;
	bb << (uint32)WoWClient->getClientBuild();
	bb.WriteDummy(4);
	bb << upperUsername;
	bb.WriteDummy(4);
	bb.Append(ourSeed.AsByteArray(4).get(), 4);
	bb.WriteDummy(20);
	bb.Append(uSHA.GetDigest(), SHA_DIGEST_LENGTH);
	bb.WriteDummy(4);
	SendData(CMSG_AUTH_SESSION, bb);

	// Start encription from here
    m_WoWCryptoUtils.Init(WoWClient->getKey());
}

void CWorldSocket::S_AuthResponse(CByteBuffer& _buff)
{
	enum CommandDetail : uint8
	{
		AuthSuccess = 0x0C,
		AuthQueue,
		AuthFailure
	};

	CommandDetail detail;
	_buff.readBytes(&detail, sizeof(uint8));

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
		Log::Green("Succes access to server!!!");
	}
	else if (detail == CommandDetail::AuthQueue)
	{
		Log::Warn("You in queue!");
	}
	else if (detail == CommandDetail::AuthFailure)
	{
		Log::Warn("Auth failed");
	}
}