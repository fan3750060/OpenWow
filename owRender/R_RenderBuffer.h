#pragma once

class RenderDevice;

struct R_RenderBuffer
{
	static const uint32 MaxColorAttachmentCount = 4;

	//

	R_RenderBuffer(RenderDevice* _RenderDevice) :
		fbo(0),
		fboMS(0),
		width(0),
		height(0),
		depthTex(0),
		depthBuf(0),
		samples(0),
		m_RenderDevice(_RenderDevice)
	{
		for (uint32 i = 0; i < MaxColorAttachmentCount; ++i)
		{
			colTexs[i] = nullptr;
			colBufs[i] = 0;
		}
	}

	//

	R_RenderBuffer* createRenderBuffer(uint32 width, uint32 height, R_TextureFormats::List format, bool depth, uint32 numColBufs, uint32 samples);
	void destroyRenderBuffer();
	R_Texture* getRenderBufferTex(uint32 bufIndex);
	void setRenderBuffer();
	void resetRenderBuffer();
	bool getRenderBufferData(int bufIndex, int *width, int *height, int *compCount, void *dataBuffer, int bufferSize);
	void getRenderBufferDimensions(int *width, int *height);

private:
	void resolveRenderBuffer();

public:
	uint32  fbo, fboMS;
	uint32  width, height;
	uint32  samples;

	R_Texture*  depthTex;
	R_Texture*  colTexs[MaxColorAttachmentCount];
	uint32      depthBuf;
	uint32      colBufs[MaxColorAttachmentCount];  // Used for multisampling

protected:
	RenderDevice* m_RenderDevice;
};