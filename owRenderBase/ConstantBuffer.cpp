#include "stdafx.h"

// General
#include "ConstantBuffer.h"

Buffer::BufferType ConstantBuffer::GetType() const
{
	return Buffer::ConstantBuffer;
}

uint32 ConstantBuffer::GetElementCount() const
{
	return 1;
}

uint32 ConstantBuffer::GetElementStride() const
{
	return 0;
}