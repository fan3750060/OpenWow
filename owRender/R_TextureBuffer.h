#pragma once

class RenderDevice;

struct R_TextureBuffer
{
public:
	R_TextureBuffer(RenderDevice* _RenderDevice) :
		bufObj(nullptr),
		glFmt(0),
		glTexID(0),
		m_RenderDevice(_RenderDevice)
	{}

	//

	R_TextureBuffer* createTextureBuffer(R_TextureFormats::List format, uint32 bufSize, const void *data, bool _isDynamic = true);
	void destroyTextureBuffer();

public:
	R_Buffer* bufObj;
	uint32    glFmt;
	uint32	  glTexID;

private:
	RenderDevice* m_RenderDevice;
};