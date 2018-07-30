#include "stdafx.h"

// General
#include "ConstantBuffer.h"

Buffer::BufferType ConstantBuffer::GetType() const
{
	return Buffer::ConstantBuffer;
}

unsigned int ConstantBuffer::GetElementCount() const
{
	return 1;
}
