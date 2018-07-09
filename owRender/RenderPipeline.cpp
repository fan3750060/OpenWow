#include "stdafx.h"

// General
#include "RenderPipeline.h"

// Additional
#include "Render.h"

RenderPipeline::RenderPipeline()
{
	_baseWidth = 320;
	_baseHeight = 240;
}


RenderPipeline::~RenderPipeline()
{
	release();
}

void RenderPipeline::release()
{
	releaseRenderTargets();

	_renderTargets.clear();
}

void RenderPipeline::resize(uint32 width, uint32 height)
{
	_baseWidth = width;
	_baseHeight = height;

	// Recreate render targets
	releaseRenderTargets();
	createRenderTargets();
}

void RenderPipeline::addRenderTarget(cstring id, bool depthBuf, uint32 numColBufs, R_TextureFormats::List format, uint32 samples, uint32 width, uint32 height, float scale)
{
	RenderTarget rt;

	rt.id = id;
	rt.hasDepthBuf = depthBuf;
	rt.numColBufs = numColBufs;
	rt.format = format;
	rt.samples = samples;
	rt.width = width;
	rt.height = height;
	rt.scale = scale;

	_renderTargets.push_back(rt);
}


RenderTarget* RenderPipeline::findRenderTarget(const string &id) const
{
	if (id == "")
	{
		return nullptr;
	}

	for (uint32 i = 0; i < _renderTargets.size(); ++i)
	{
		if (_renderTargets[i].id == id)
		{
			return (RenderTarget*)&_renderTargets[i];
		}
	}

	return nullptr;
}

bool RenderPipeline::getRenderTargetData(cstring target, int bufIndex, int *width, int *height, int *compCount, void *dataBuffer, int bufferSize) const
{
	R_RenderBuffer* rbObj = 0;
	if (target != "")
	{
		RenderTarget* rt = findRenderTarget(target);
		if (rt == nullptr)
		{
			return false;
		}
		else
		{
			rbObj = rt->rendBuf;
		}
	}

	return rbObj->getRenderBufferData(bufIndex, width, height, compCount, dataBuffer, bufferSize);
}

//--

bool RenderPipeline::createRenderTargets()
{
	RenderDevice* rdi = &_Render->r;

	for (uint32 i = 0; i < _renderTargets.size(); ++i)
	{
		RenderTarget& rt = _renderTargets[i];

		uint32 width = ftoi_r(rt.width * rt.scale);
		uint32 height = ftoi_r(rt.height * rt.scale);

		if (width == 0)
		{
			width = ftoi_r(_baseWidth * rt.scale);
		}

		if (height == 0)
		{
			height = ftoi_r(_baseHeight * rt.scale);
		}

		rt.rendBuf = rdi->createRenderBuffer(width, height, rt.format, rt.hasDepthBuf, rt.numColBufs, rt.samples);
		if (rt.rendBuf == nullptr)
		{
			return false;
		}
	}

	return true;
}


void RenderPipeline::releaseRenderTargets()
{
	for (uint32 i = 0; i < _renderTargets.size(); ++i)
	{
		RenderTarget& rt = _renderTargets[i];
		if (rt.rendBuf)
		{
			delete rt.rendBuf;
			rt.rendBuf = nullptr;
		}
	}
}

