#pragma once

class RenderDevice;

struct R_TextureBuffer
{
public:
	R_TextureBuffer(RenderDevice* _RenderDevice);
	~R_TextureBuffer();

	//

	R_TextureBuffer* createTextureBuffer(R_TextureFormats::List format, uint32 bufSize, const void *data, bool _isDynamic = true);

public:
	SharedBufferPtr	m_Buffer;
	uint32			m_GLFmt;
	uint32			m_GLTexID;

private:
	RenderDevice* m_RenderDevice;
};