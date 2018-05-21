#include "stdafx.h"

// General
#include "R_Buffer.h"

// Additional
#include "OpenGL.h"

static const uint32 bufferMappingTypes[3] = { GL_MAP_READ_BIT, GL_MAP_WRITE_BIT, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT };

R_Buffer* R_Buffer::createBuffer(uint32 bufType, uint32 size, const void *data, bool _isDynamic)
{
	this->type = bufType;
	this->size = size;
	this->isDynamic = _isDynamic;

	glGenBuffers(1, &this->glObj);
	glBindBuffer(this->type, this->glObj);
	glBufferData(this->type, size, data, _isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(this->type, 0);

	m_RenderDevice->_bufferMem += size;
	return this;
}

R_Buffer* R_Buffer::createVertexBuffer(uint32 size, const void *data, bool _isDynamic)
{
	return createBuffer(GL_ARRAY_BUFFER, size, data, _isDynamic);
}

R_Buffer* R_Buffer::createIndexBuffer(uint32 size, const void *data, bool _isDynamic)
{
	return createBuffer(GL_ELEMENT_ARRAY_BUFFER, size, data, _isDynamic);
}

R_Buffer* R_Buffer::createShaderStorageBuffer(uint32 size, const void *data, bool _isDynamic)
{
	if (_Config.DeviceCaps.computeShaders)
	{
		return createBuffer(GL_SHADER_STORAGE_BUFFER, size, data, _isDynamic);
	}
	else
	{
		Log::Error("Shader storage buffers are not supported on this OpenGL 4 device.");

		return 0;
	}
}

void R_Buffer::updateBufferData(uint32 offset, uint32 size, const void *data)
{
	assert1(this->isDynamic);
	assert1(offset + size <= this->size);

	glBindBuffer(this->type, this->glObj);

	if (offset == 0 && size == this->size)
	{
		// Replacing the whole buffer can help the driver to avoid pipeline stalls
		glBufferData(this->type, size, data, GL_DYNAMIC_DRAW);
		return;
	}

	glBufferSubData(this->type, offset, size, data);
}

void* R_Buffer::mapBuffer(uint32 offset, uint32 size, R_BufferMappingTypes mapType)
{
	assert1(this->isDynamic);
	assert1(offset + size <= this->size);

	glBindBuffer(this->type, this->glObj);

	if (offset == 0 && size == this->size && mapType == Write)
	{
		return glMapBufferRange(this->type, offset, size, bufferMappingTypes[mapType] | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	}

	return glMapBufferRange(this->type, offset, size, bufferMappingTypes[mapType]);
}

void R_Buffer::unmapBuffer()
{
	assert1(this->isDynamic);

	// multiple buffers can be mapped at the same time, so bind the one that needs to be unmapped
	glBindBuffer(this->type, this->glObj);

	glUnmapBuffer(this->type);
}

void R_Buffer::destroyBuffer()
{
	if (this->geometryRefCount < 1)
	{
		glDeleteBuffers(1, &this->glObj);
		m_RenderDevice->_bufferMem -= this->size;
	}
}

void R_Buffer::decreaseBufferRefCount()
{
	this->geometryRefCount--;
}