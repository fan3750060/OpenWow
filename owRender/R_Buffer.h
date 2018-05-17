#pragma once

class RenderDevice;
#include "R_BufferBase.h"

struct R_Buffer : public R_BufferBase
{
	R_Buffer(RenderDevice* _RenderDevice) :
		R_BufferBase(_RenderDevice)
	{}

	//

	R_Buffer* createBuffer(uint32 bufType, uint32 size, const void *data, bool _isDynamic = true);
	R_Buffer* createVertexBuffer(uint32 size, const void *data, bool _isDynamic = true);
	R_Buffer* createIndexBuffer(uint32 size, const void *data, bool _isDynamic = true);
	R_Buffer* createShaderStorageBuffer(uint32 size, const void *data, bool _isDynamic = true);
	
	void updateBufferData(uint32 offset, uint32 size, const void *data);
	void* mapBuffer(uint32 offset, uint32 size, R_BufferMappingTypes mapType);
	void unmapBuffer();
};