#pragma once

struct R_BufferBase
{
public:
	R_BufferBase(RenderDevice* _RenderDevice) :
		type(0),
		glObj(0),
		size(0),
		isDynamic(true),
		geometryRefCount(0),
		m_RenderDevice(_RenderDevice)
	{}

	void destroyBuffer();
	void decreaseBufferRefCount();

public:
	uint32  type;
	uint32  glObj;
	uint32  size;
	bool    isDynamic;
	int geometryRefCount;

protected:
	RenderDevice* m_RenderDevice;
};