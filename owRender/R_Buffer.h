#pragma once

class RenderDevice;

struct R_Buffer : public CRefItem
{
	R_Buffer(RenderDevice* _RenderDevice) :
		type(0),
		glObj(0),
		size(0),
		isDynamic(true),
		geometryRefCount(0),
		m_RenderDevice(_RenderDevice)
	{}

	//

	R_Buffer* createBuffer(uint32 bufType, uint32 size, const void *data, bool _isDynamic = true);
	R_Buffer* createVertexBuffer(uint32 size, const void *data, bool _isDynamic = true);
	R_Buffer* createIndexBuffer(uint32 size, const void *data, bool _isDynamic = true);
	R_Buffer* createShaderStorageBuffer(uint32 size, const void *data, bool _isDynamic = true);
	
	void updateBufferData(uint32 offset, uint32 size, const void *data);
	void* mapBuffer(uint32 offset, uint32 size, R_BufferMappingTypes mapType);
	void unmapBuffer();

	void destroyBuffer();
	void decreaseBufferRefCount();

public:
	uint32  type;
	uint32  glObj;
	uint32  size;
	bool    isDynamic;
	int geometryRefCount;

private:
	RenderDevice* m_RenderDevice;
};

struct R_BufferDeleter
{
	void operator()(R_Buffer* p)
	{
		p->destroyBuffer();
		delete p;
	}
};
typedef SmartPtr<R_Buffer, R_BufferDeleter> SmartBufferPtr;