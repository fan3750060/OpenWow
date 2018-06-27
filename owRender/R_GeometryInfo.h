#pragma once

class RenderDevice;

struct R_VertexBufferSlot
{
	R_VertexBufferSlot() :
		m_VertexBuffer(nullptr),
		m_VertexBufferDataType(R_DataType::T_FLOAT),
		m_Offset(0),
		m_Stride(0),
		m_NeedNorm(false)
	{}

	R_VertexBufferSlot(R_Buffer* _vbObj, R_DataType _type, uint32 _offset, uint32 _stride, bool _needNorm) :
		m_VertexBuffer(_vbObj),
		m_VertexBufferDataType(_type),
		m_Offset(_offset),
		m_Stride(_stride),
		m_NeedNorm(_needNorm)
	{}

	//

	R_Buffer*   m_VertexBuffer;
	R_DataType  m_VertexBufferDataType;
	uint32      m_Offset;
	uint32      m_Stride;
	bool        m_NeedNorm;
};

class R_GeometryInfo : public CRefItem
{
public:
	R_GeometryInfo(RenderDevice* _RenderDevice);
	~R_GeometryInfo();

	//

	void setGeomVertexParams(R_Buffer* _vbo, R_DataType _type, uint32 _offset, uint32 _stride, bool _needNorm = false);
	void setGeomIndexParams(R_Buffer* _indBuf, R_IndexFormat _format);
	void finishCreatingGeometry();

public:
	vector<R_VertexBufferSlot>	m_VertexBufInfo;
	uint32						m_VAOGLObj;
	SmartBufferPtr				m_IndexBuffer;
	uint32						m_Layout;
	R_IndexFormat				m_IndexBufferFormat;
	bool						m_AtrribsBinded;

protected:
	RenderDevice*				m_RenderDevice;
};

typedef SmartPtr<R_GeometryInfo> SmartGeomPtr;