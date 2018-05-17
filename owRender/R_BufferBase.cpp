#include "stdafx.h"

// General
#include "R_BufferBase.h"

// Additional
#include "OpenGL.h"

void R_BufferBase::destroyBuffer()
{
	if (this->geometryRefCount < 1)
	{
		glDeleteBuffers(1, &this->glObj);
		m_RenderDevice->_bufferMem -= this->size;
	}
}

void R_BufferBase::decreaseBufferRefCount()
{
	this->geometryRefCount--;
}