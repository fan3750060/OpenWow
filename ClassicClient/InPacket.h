#pragma once

#include "Opcodes.h"

struct InPacket
{
	Opcodes		opcode;
	uint32		size;
	ByteBuffer	data;
};