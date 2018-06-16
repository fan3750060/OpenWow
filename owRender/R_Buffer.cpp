#include "stdafx.h"

// General
#include "R_Buffer.h"

// Additional
#include "OpenGL.h"

static const uint32 bufferMappingTypes[3] = { GL_MAP_READ_BIT, GL_MAP_WRITE_BIT, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT };

R_Buffer::R_Buffer(RenderDevice* _RenderDevice) :
	m_Type(0),
	m_GLObj(0),
	m_Size(0),
	m_IsDynamic(true),
	m_RenderDevice(_RenderDevice)
{}

R_Buffer::~R_Buffer()
{
	glDeleteBuffers(1, &m_GLObj);
	m_RenderDevice->m_BufferMem -= m_Size;
}

R_Buffer* R_Buffer::createBuffer(uint32 _bufType, uint32 _size, const void* _data, bool _isDynamic)
{
	m_Type = _bufType;
	m_Size = _size;
	m_IsDynamic = _isDynamic;

	glGenBuffers(1, &m_GLObj);
	glBindBuffer(m_Type, m_GLObj);
	glBufferData(m_Type, _size, _data, _isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(m_Type, 0);

	m_RenderDevice->m_BufferMem += _size;
	return this;
}

R_Buffer* R_Buffer::createVertexBuffer(uint32 _size, const void* _data, bool _isDynamic)
{
	return createBuffer(GL_ARRAY_BUFFER, _size, _data, _isDynamic);
}

R_Buffer* R_Buffer::createIndexBuffer(uint32 _size, const void* _data, bool _isDynamic)
{
	return createBuffer(GL_ELEMENT_ARRAY_BUFFER, _size, _data, _isDynamic);
}

R_Buffer* R_Buffer::createShaderStorageBuffer(uint32 _size, const void* _data, bool _isDynamic)
{
	if (GetSettingsGroup<CGroupRenderCaps>().computeShaders)
	{
		return createBuffer(GL_SHADER_STORAGE_BUFFER, _size, _data, _isDynamic);
	}
	else
	{
		Log::Error("Shader storage buffers are not supported on this OpenGL 4 device.");
		return 0;
	}
}

void R_Buffer::updateBufferData(uint32 _offset, uint32 _size, const void* _data)
{
	assert1(m_IsDynamic);
	assert1(_offset + _size <= m_Size);

	glBindBuffer(m_Type, m_GLObj);

	if (_offset == 0 && _size == m_Size)
	{
		// Replacing the whole buffer can help the driver to avoid pipeline stalls
		glBufferData(m_Type, _size, _data, GL_DYNAMIC_DRAW);
		return;
	}

	glBufferSubData(m_Type, _offset, _size, _data);
}

void* R_Buffer::mapBuffer(uint32 _offset, uint32 _size, R_BufferMappingTypes _mapType)
{
	assert1(m_IsDynamic);
	assert1(_offset + _size <= m_Size);

	glBindBuffer(m_Type, m_GLObj);

	if (_offset == 0 && _size == m_Size && _mapType == Write)
	{
		return glMapBufferRange(m_Type, _offset, _size, bufferMappingTypes[_mapType] | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	}

	return glMapBufferRange(m_Type, _offset, _size, bufferMappingTypes[_mapType]);
}

void R_Buffer::unmapBuffer()
{
	assert1(m_IsDynamic);

	// multiple buffers can be mapped at the same time, so bind the one that needs to be unmapped
	glBindBuffer(m_Type, m_GLObj);

	glUnmapBuffer(m_Type);
}