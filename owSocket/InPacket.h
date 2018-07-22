#pragma once

#include "Opcodes.h"

class InPacketBuilder
{
public:

};

struct InPacket
{
	uint32		dataSize;
	Opcodes		opcode;
	ByteBuffer	data;
};