#include "stdafx.h"

// Include
#include "AuthWorldController.h"

// General
#include "WorldSocket.h"

// Additional
#include "OpcodesNames.h"

const Opcodes IgnoredOpcodes[] = 
{
	Opcodes::SMSG_POWER_UPDATE,
	Opcodes::SMSG_SET_PROFICIENCY,
	Opcodes::MSG_SET_DUNGEON_DIFFICULTY,
	Opcodes::SMSG_LOGIN_VERIFY_WORLD,
	Opcodes::SMSG_ACCOUNT_DATA_TIMES,
	Opcodes::SMSG_FEATURE_SYSTEM_STATUS,
	Opcodes::SMSG_LEARNED_DANCE_MOVES,
	Opcodes::SMSG_BINDPOINTUPDATE,
	Opcodes::SMSG_TALENTS_INFO,
	Opcodes::SMSG_INSTANCE_DIFFICULTY,
	Opcodes::SMSG_INITIAL_SPELLS,
	Opcodes::SMSG_SEND_UNLEARN_SPELLS,
	Opcodes::SMSG_ACTION_BUTTONS,
	Opcodes::SMSG_INITIALIZE_FACTIONS,
	Opcodes::SMSG_ALL_ACHIEVEMENT_DATA,
	Opcodes::SMSG_EQUIPMENT_SET_LIST,
	Opcodes::SMSG_LOGIN_SETTIMESPEED,
	Opcodes::SMSG_SET_FORCED_REACTIONS,
	Opcodes::SMSG_COMPRESSED_UPDATE_OBJECT,
	Opcodes::SMSG_MONSTER_MOVE,

	Opcodes::SMSG_EMOTE,
	Opcodes::SMSG_TIME_SYNC_REQ,
	Opcodes::SMSG_SPELL_START,
	Opcodes::SMSG_SPELL_GO
};

CWorldSocket::CWorldSocket(CAuthWorldController* _world) :
	m_World(_world),
	m_Realm(nullptr),
	socketBase(nullptr),
	currPacket(nullptr)
{
	
}

CWorldSocket::~CWorldSocket()
{
	m_ThreadPromise.set_value();
	if (m_Thread.joinable()) m_Thread.join();

	Log::Info("[WorldSocket]: All threads stopped.");

	delete socketBase;
}

void CWorldSocket::Create(RealmInfo* _realm)
{
	m_Realm = _realm;
	socketBase = new CSocketBase(m_Realm->getIP(), m_Realm->getPort());
}

//--

void CWorldSocket::SendData(Opcodes _opcode)
{
	Opcodes command = (Opcodes)((&reinterpret_cast<uint8&>(_opcode))[1] << 8 | (&reinterpret_cast<uint8&>(_opcode))[0]);

	ByteBuffer finalBuffer;
	finalBuffer << (uint8)0;
	finalBuffer << (uint8)4;
	finalBuffer << (uint32)command;
	SendData(finalBuffer.getData(), finalBuffer.getSize());
}
void CWorldSocket::SendData(Opcodes _opcode, ByteBuffer& _bb)
{
	assert1((_bb.getSize() + 4) < UINT16_MAX);

	uint16 size0 = _bb.getSize() + 4 /* HEADER */;
	uint16 sizeFinal = ((&reinterpret_cast<uint8&>(size0))[0] << 8 | (&reinterpret_cast<uint8&>(size0))[1]);
	Opcodes command = (Opcodes)((&reinterpret_cast<uint8&>(_opcode))[1] << 8 | (&reinterpret_cast<uint8&>(_opcode))[0]);

	ByteBuffer finalBuffer;
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

	cryptUtils.EncryptSend(data2, 6 /* size (2) + header (4)*/);

	socketBase->SendData(data2, _count);
}

void CWorldSocket::WorldThread(std::future<void> futureObj)
{
	while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		if (socketBase->getReadCache()->isReady())
		{
			OnDataReceive(socketBase->getReadCache()->Pop());
		}
	}

	Log::Info("[WorldSocket]: Exit thread.");
}

void CWorldSocket::InitHandlers()
{
	m_Handlers[SMSG_AUTH_CHALLENGE] = FUNCTION_CLASS_WA_Builder(CWorldSocket, this, S_AuthChallenge, ByteBuffer&);
	m_Handlers[SMSG_AUTH_RESPONSE] = FUNCTION_CLASS_WA_Builder(CWorldSocket, this, S_AuthResponse, ByteBuffer&);
	
	// Dummy
	m_Handlers[SMSG_POWER_UPDATE] = nullptr;
	m_Handlers[SMSG_SET_PROFICIENCY] = nullptr;
	m_Handlers[MSG_SET_DUNGEON_DIFFICULTY] = nullptr;
	m_Handlers[SMSG_LOGIN_VERIFY_WORLD] = nullptr;
	m_Handlers[SMSG_ACCOUNT_DATA_TIMES] = nullptr;
	m_Handlers[SMSG_FEATURE_SYSTEM_STATUS] = nullptr;
	m_Handlers[SMSG_LEARNED_DANCE_MOVES] = nullptr;
	m_Handlers[SMSG_BINDPOINTUPDATE] = nullptr;
	m_Handlers[SMSG_TALENTS_INFO] = nullptr;
	m_Handlers[SMSG_INSTANCE_DIFFICULTY] = nullptr;
	m_Handlers[SMSG_INITIAL_SPELLS] = nullptr;
	m_Handlers[SMSG_SEND_UNLEARN_SPELLS] = nullptr;
	m_Handlers[SMSG_ACTION_BUTTONS] = nullptr;
	m_Handlers[SMSG_INITIALIZE_FACTIONS] = nullptr;
	m_Handlers[SMSG_ALL_ACHIEVEMENT_DATA] = nullptr;
	m_Handlers[SMSG_EQUIPMENT_SET_LIST] = nullptr;
	m_Handlers[SMSG_LOGIN_SETTIMESPEED] = nullptr;
	m_Handlers[SMSG_SET_FORCED_REACTIONS] = nullptr;
	m_Handlers[SMSG_COMPRESSED_UPDATE_OBJECT] = nullptr;
	m_Handlers[SMSG_MONSTER_MOVE] = nullptr;

	m_Handlers[Opcodes::SMSG_EMOTE] = nullptr;
	m_Handlers[Opcodes::SMSG_TIME_SYNC_REQ] = nullptr;
	m_Handlers[Opcodes::SMSG_SPELL_START] = nullptr;
	m_Handlers[Opcodes::SMSG_SPELL_GO] = nullptr;

	// Thread
	std::future<void> futureObj = m_ThreadPromise.get_future();
	m_Thread = std::thread(&CWorldSocket::WorldThread, this, std::move(futureObj));
	m_Thread.detach();

	// Event
	//socketBase->setOnReceiveCallback(FUNCTION_CLASS_WA_Builder(CWorldSocket, this, OnDataReceive, ByteBuffer));
}

void CWorldSocket::OnDataReceive(ByteBuffer _buf)
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
		cryptUtils.DecryptRecv(data + 0, 1);
		uint8 firstByte = data[0];

		// 2. Decrypt other size
		if ((firstByte & 0x80) != 0)
		{
			sizeCorrection = 3;
			cryptUtils.DecryptRecv(data + 1, 1 + sizeCorrection);
			packetSize = (/*((data[0] & 0x7F) << 16) |*/ (data[1] << 8) | data[2]);
			command = ((data[4] << 8) | data[3]);
		}
		else
		{
			sizeCorrection = 2;
			cryptUtils.DecryptRecv(data + 1, 1 + sizeCorrection);
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

void CWorldSocket::AddHandler(Opcodes _opcode, Function_WA<ByteBuffer&>* _func)
{
	assert1(_func != nullptr);
	m_Handlers.insert(std::make_pair(_opcode, _func));
}

void CWorldSocket::ProcessHandler(Opcodes _handler, ByteBuffer _buffer)
{
	std::unordered_map<Opcodes, Function_WA<ByteBuffer&>*>::iterator handler = m_Handlers.find(_handler);
	if (handler != m_Handlers.end())
	{
		if (handler->second != nullptr)
		{
			(handler->second)->operator()(_buffer);
		}
	}
}

//--

void CWorldSocket::Packet1(uint16 _command, uint32 _size)
{
	ByteBuffer buff;

	currPacket = new InPacket();
	currPacket->dataSize = _size;
	currPacket->opcode = (Opcodes)_command;
}

void CWorldSocket::Packet2(ByteBuffer& _buf)
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
	//Log::Info("Packet[%s] readed '%d' of '%d'.", OpcodesNames[currPacket->opcode].c_str(), currPacket->data.getSize(), currPacket->dataSize);

	// Final
	if (currPacket->data.getSize() == currPacket->dataSize)
	{
		ProcessHandler(currPacket->opcode, currPacket->data);

		SafeDelete(currPacket);
	}
}

//--

void CWorldSocket::S_AuthChallenge(ByteBuffer& _buff)
{
	uint32 one;
	_buff.readBytes(&one, 4);

	uint32 seed; 
	_buff.readBytes(&seed, 4);

	_buff.seekRelative(24);
	
	BigNumber ourSeed;
	ourSeed.SetRand(4 * 8);

	std::string upperUsername = Utils::ToUpper(m_World->getUsername());
	uint8 zeroBytes[] = { 0, 0, 0, 0 };

	SHA1Hash uSHA;
	uSHA.Initialize();
	uSHA.UpdateData((const uint8*)upperUsername.c_str(), upperUsername.size());
	uSHA.UpdateData(zeroBytes, 4);
	uSHA.UpdateBigNumbers(&ourSeed, nullptr);
	uSHA.UpdateData(&reinterpret_cast<uint8&>(seed), 4);
	uSHA.UpdateBigNumbers(m_World->getKey(), nullptr);
	uSHA.Finalize();

	//------------------
	ByteBuffer bb;
	bb << (uint32)12345;
	bb.WriteDummy(4);
	bb << upperUsername;
	bb.WriteDummy(4);
	bb.Append(ourSeed.AsByteArray(4).get(), 4);
	bb.WriteDummy(20);
	bb.Append(uSHA.GetDigest(), SHA_DIGEST_LENGTH);
	bb.WriteDummy(4);
	SendData(CMSG_AUTH_SESSION, bb);

	// Start encription from here
	cryptUtils.Init(m_World->getKey());
}

void CWorldSocket::S_AuthResponse(ByteBuffer& _buff)
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