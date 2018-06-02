#pragma once

class RenderDevice;

struct R_GeometryInfo : public CRefItem
{
	R_GeometryInfo(RenderDevice* _RenderDevice) :
		vao(0),
		indexBuf(nullptr),
		layout(0),
		indexBuf32Bit(false),
		atrribsBinded(false),
		m_RenderDevice(_RenderDevice)
	{}

	//

	void finishCreatingGeometry();
	void setGeomVertexParams(R_Buffer* vbo, R_DataType type, uint32 offset, uint32 stride, bool needNorm = false);
	void setGeomIndexParams(R_Buffer* indBuf, R_IndexFormat format);
	void destroyGeometry(bool destroyBindedBuffers);

public:
	vector<R_VertexBufferSlot> vertexBufInfo;
	uint32 vao;
	R_Buffer* indexBuf;
	uint32 layout;
	bool indexBuf32Bit;
	bool atrribsBinded;

protected:
	RenderDevice* m_RenderDevice;
};

struct R_GeomDeleter
{
	void operator()(R_GeometryInfo* p)
	{
		p->destroyGeometry(true);
		delete p;
	}
};
typedef SmartPtr<R_GeometryInfo, R_GeomDeleter> SmartGeomPtr;