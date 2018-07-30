#pragma once

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

	SharedBufferPtr	m_VertexBuffer;
	R_DataType		m_VertexBufferDataType;
	uint32			m_Offset;
	uint32			m_Stride;
	bool			m_NeedNorm;
};

// FORWARD BEGIN
class RenderDevice;
// FORWARD END

class R_GeometryInfo
{
public:
	R_GeometryInfo(RenderDevice* _RenderDevice);
	R_GeometryInfo(const R_GeometryInfo& _other);
	virtual ~R_GeometryInfo();

	//

	void setGeomVertexParams(SharedBufferPtr _vbo, R_DataType _type, uint32 _offset, uint32 _stride, bool _needNorm = false);
	void setGeomIndexParams(SharedBufferPtr _indBuf, R_IndexFormat _format);
	void finishCreatingGeometry();

public:
	vector<R_VertexBufferSlot>	m_VertexBufInfo;
	uint32						m_VAOGLObj;
	SharedBufferPtr				m_IndexBuffer;
	R_PrimitiveType				m_PrimType;
	uint32						m_VertexLayout;
	R_IndexFormat				m_IndexBufferFormat;
	bool						m_AtrribsBinded;

protected:
	RenderDevice*				m_RenderDevice;
};

typedef shared_ptr<R_GeometryInfo> SharedGeomPtr;