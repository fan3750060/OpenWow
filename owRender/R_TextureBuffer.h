#pragma once

class RenderDevice;
#include "R_BufferBase.h"

struct R_TextureBuffer : public R_BufferBase
{
public:
	R_TextureBuffer(RenderDevice* _RenderDevice) :
		R_BufferBase(_RenderDevice),
		bufObj(nullptr),
		glFmt(0),
		glTexID(0)
	{}

	//

	R_TextureBuffer* createTextureBuffer(R_TextureFormats::List format, uint32 bufSize, const void *data, bool _isDynamic = true);
	void destroyTextureBuffer();

public:
	R_Buffer* bufObj;
	uint32    glFmt;
	uint32	  glTexID;
};