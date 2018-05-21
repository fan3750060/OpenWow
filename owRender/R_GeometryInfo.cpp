#include "stdafx.h"

// General
#include "R_GeometryInfo.h"

// Additional
#include "OpenGL.h"

static const uint32 dataTypes[8] = { GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE };

void R_GeometryInfo::finishCreatingGeometry()
{
	glBindVertexArray(this->vao);

	// bind index buffer, if present
	if (this->indexBuf)
	{
		assert1(this->indexBuf->glObj > 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuf->glObj);
	}

	uint32 newVertexAttribMask = 0;
	R_VertexLayout vl = m_RenderDevice->_vertexLayouts[this->layout - 1];

	// Set vertex attrib pointers
	for (uint32 i = 0; i < vl.numAttribs; ++i)
	{
		R_VertexLayoutAttrib& attrib = vl.attribs[i];
		const R_VertexBufferSlot& vbSlot = this->vertexBufInfo[attrib.vbSlot];

		R_Buffer* buf = this->vertexBufInfo[attrib.vbSlot].vbObj;
		assert1(buf->glObj && buf->type == GL_ARRAY_BUFFER || buf->type == GL_SHADER_STORAGE_BUFFER); // special case for compute buffer

		glBindBuffer(GL_ARRAY_BUFFER, buf->glObj);
		glVertexAttribPointer(i, attrib.size, dataTypes[vbSlot.type], vbSlot.needNorm, vbSlot.stride, (char*)0 + vbSlot.offset + attrib.offset);

		newVertexAttribMask |= 1 << i;
	}


	for (uint32 i = 0; i < 16; ++i)
	{
		uint32 curBit = 1 << i;
		if ((newVertexAttribMask & curBit) != (m_RenderDevice->_activeVertexAttribsMask & curBit))
		{
			if (newVertexAttribMask & curBit)
			{
				glEnableVertexAttribArray(i);
			}
			else
			{
				glDisableVertexAttribArray(i);
			}
		}
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void R_GeometryInfo::setGeomVertexParams(R_Buffer* vbo, R_DataType type, uint32 offset, uint32 stride, bool needNorm)
{
	vbo->geometryRefCount++;

	R_VertexBufferSlot attribInfo;
	attribInfo.vbObj = vbo;
	attribInfo.type = type;
	attribInfo.offset = offset;
	attribInfo.stride = stride;
	attribInfo.needNorm = needNorm;

	this->vertexBufInfo.push_back(attribInfo);
}

void R_GeometryInfo::setGeomIndexParams(R_Buffer* indBuf, R_IndexFormat format)
{
	indBuf->geometryRefCount++;
	this->indexBuf = indBuf;
	this->indexBuf32Bit = (format == IDXFMT_32 ? true : false);
}

void R_GeometryInfo::destroyGeometry(bool destroyBindedBuffers)
{
	glDeleteVertexArrays(1, &this->vao);
	glBindVertexArray(0);

	if (destroyBindedBuffers)
	{
		for (unsigned int i = 0; i < this->vertexBufInfo.size(); ++i)
		{
			this->vertexBufInfo[i].vbObj->decreaseBufferRefCount();
			this->vertexBufInfo[i].vbObj->destroyBuffer();
		}

		if (this->indexBuf)
		{
			this->indexBuf->decreaseBufferRefCount();
			this->indexBuf->destroyBuffer();
		}
	}
	else
	{
		for (uint32_t i = 0; i < this->vertexBufInfo.size(); ++i)
		{
			this->vertexBufInfo[i].vbObj->decreaseBufferRefCount();
		}

		if (this->indexBuf)
		{
			this->indexBuf->decreaseBufferRefCount();
		}
	}
}