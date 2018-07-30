#include "stdafx.h"

// General
#include "R_GeometryInfo.h"

// Additional
#include "OpenGL.h"

static const uint32 dataTypes[8] = { GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE };

R_GeometryInfo::R_GeometryInfo(RenderDevice* _RenderDevice) :
	m_VAOGLObj(0),
	m_IndexBuffer(nullptr),
	m_VertexLayout(0),
	m_IndexBufferFormat(R_IndexFormat::IDXFMT_16),
	m_AtrribsBinded(false),
	m_RenderDevice(_RenderDevice)
{}
R_GeometryInfo::R_GeometryInfo(const R_GeometryInfo & _other)
{
	if (this != &_other)
	{
		m_VertexBufInfo = _other.m_VertexBufInfo;
		m_VAOGLObj = _other.m_VAOGLObj;
		m_IndexBuffer = _other.m_IndexBuffer;
		m_PrimType = _other.m_PrimType;
		m_VertexLayout = _other.m_VertexLayout;
		m_IndexBufferFormat = _other.m_IndexBufferFormat;
		m_AtrribsBinded = _other.m_AtrribsBinded;
		m_RenderDevice = _other.m_RenderDevice;
	}
}
R_GeometryInfo::~R_GeometryInfo()
{
	glDeleteVertexArrays(1, &m_VAOGLObj);
	glBindVertexArray(0);
}

void R_GeometryInfo::setGeomVertexParams(SharedBufferPtr _vbo, R_DataType _type, uint32 _offset, uint32 _stride, bool _needNorm)
{
	R_VertexBufferSlot attribInfo;
	attribInfo.m_VertexBuffer = _vbo;
	attribInfo.m_VertexBufferDataType = _type;
	attribInfo.m_Offset = _offset;
	attribInfo.m_Stride = _stride;
	attribInfo.m_NeedNorm = _needNorm;

	m_VertexBufInfo.push_back(attribInfo);
}

void R_GeometryInfo::setGeomIndexParams(SharedBufferPtr _indBuf, R_IndexFormat _format)
{
	m_IndexBuffer = _indBuf;
	m_IndexBufferFormat = _format;
}

void R_GeometryInfo::finishCreatingGeometry()
{
	glBindVertexArray(m_VAOGLObj);

	// bind index buffer, if present
	if (m_IndexBuffer)
	{
		assert1(m_IndexBuffer->m_GLObj > 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer->m_GLObj);
	}

	uint32 newVertexAttribMask = 0;
	R_VertexLayout vl = m_RenderDevice->m_VertexLayouts[m_VertexLayout - 1];

	// Set vertex attrib pointers
	for (uint32 i = 0; i < vl.numAttribs; ++i)
	{
		R_VertexLayoutAttrib& attrib = vl.attribs[i];
		const R_VertexBufferSlot& vbSlot = m_VertexBufInfo[attrib.vbSlot];

		SharedBufferPtr buf = m_VertexBufInfo[attrib.vbSlot].m_VertexBuffer;
		assert1(buf->m_GLObj && buf->m_Type == GL_ARRAY_BUFFER || buf->m_Type == GL_SHADER_STORAGE_BUFFER); // special case for compute buffer

		glBindBuffer(GL_ARRAY_BUFFER, buf->m_GLObj);
		glVertexAttribPointer(i, attrib.size, dataTypes[vbSlot.m_VertexBufferDataType], vbSlot.m_NeedNorm, vbSlot.m_Stride, (char*)0 + vbSlot.m_Offset + attrib.offset);

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