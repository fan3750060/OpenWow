#pragma once

class RenderDevice;

class R_Buffer : public CRefItem
{
public:
	R_Buffer(RenderDevice* _RenderDevice);
	~R_Buffer();

	//

	R_Buffer* createBuffer(uint32 _bufType, uint32 _size, const void* _data, bool _isDynamic = true);
	R_Buffer* createVertexBuffer(uint32 _size, const void* _data, bool _isDynamic = true);
	R_Buffer* createIndexBuffer(uint32 _size, const void* _data, bool _isDynamic = true);
	R_Buffer* createShaderStorageBuffer(uint32 _size, const void* _data, bool _isDynamic = true);
	
	void updateBufferData(uint32 _offset, uint32 _size, const void* _data);
	void* mapBuffer(uint32 _offset, uint32 _size, R_BufferMappingTypes _mapType);
	void unmapBuffer();

public:
	uint32  m_Type;
	uint32  m_GLObj;
	uint32  m_Size;
	bool    m_IsDynamic;

private:
	RenderDevice* m_RenderDevice;
};

struct R_BufferDeleter
{
	void operator()(R_Buffer* p)
	{
		delete p;
	}
};
typedef SmartPtr<R_Buffer, R_BufferDeleter> SmartBufferPtr;