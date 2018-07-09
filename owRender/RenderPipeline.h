#pragma once

#include "RenderEnums.h"

struct RenderTarget
{
	RenderTarget()
	{
		id = "";
		numColBufs = 0;
		format = R_TextureFormats::Unknown;
		width = height = 0;
		samples = 0;
		scale = 0;
		hasDepthBuf = false;
		rendBuf = nullptr;
	}

	string					id;
	uint32					numColBufs;
	R_TextureFormats::List	format;
	uint32					width, height;
	uint32					samples;
	float					scale;  // Scale factor for FB width and height
	bool					hasDepthBuf;
	R_RenderBuffer*			rendBuf;
};

class RenderPipeline
{
public:
	RenderPipeline();
	~RenderPipeline();

	void release();
	void resize(uint32 width, uint32 height);

	void addRenderTarget(cstring id, bool depthBuffer, uint32 numBuffers, R_TextureFormats::List format, uint32 samples, uint32 width, uint32 height, float scale);
	RenderTarget* findRenderTarget(const std::string &id) const;
	bool getRenderTargetData(cstring target, int bufIndex, int *width, int *height, int *compCount, void *dataBuffer, int bufferSize) const;

private:
	bool createRenderTargets();
	void releaseRenderTargets();

private:
	vector<RenderTarget>   _renderTargets;
	uint32                 _baseWidth, _baseHeight;
};