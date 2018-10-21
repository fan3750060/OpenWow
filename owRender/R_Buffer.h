#pragma once

class RenderDevice;

class R_Buffer
{
public:
	R_Buffer(RenderDevice* _RenderDevice);
	R_Buffer(const R_Buffer& _other);
	virtual ~R_Buffer();

	//

	void createBuffer(uint32 _bufType, uint32 _size, const void* _data, bool _isDynamic = true);
	void createVertexBuffer(uint32 _size, const void* _data, bool _isDynamic = true);
	void createIndexBuffer(uint32 _size, const void* _data, bool _isDynamic = true);
	void createShaderStorageBuffer(uint32 _size, const void* _data, bool _isDynamic = true);
	
	void updateBufferData(uint32 _offset, uint32 _size, const void* _data);
	void* mapBuffer(uint32 _offset, uint32 _size, R_BufferMappingTypes _mapType);
	void unmapBuffer();

	uint32 getSize() const { return m_Size; }

public:
	uint32  m_Type;
	uint32  m_GLObj;
	uint32  m_Size;
	bool    m_IsDynamic;

private:
	RenderDevice* m_RenderDevice;
};

typedef std::shared_ptr<R_Buffer> SharedBufferPtr;
