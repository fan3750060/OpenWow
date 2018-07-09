#include "stdafx.h"

// General
#include "ShaderContext.h"

// Additional
#include "Render.h"

ShaderContext::ShaderContext() :
	// Culling
	cullMode(R_CullMode::RS_CULL_BACK),
	// Alpha to coverage
	alphaToCoverage(false),
	// Blend
	blendingEnabled(false),
	blendStateRGBSrc(R_BlendFunc::BS_BLEND_ZERO),
	blendStateRGBDst(R_BlendFunc::BS_BLEND_ZERO),
	blendStateASrc(R_BlendFunc::BS_BLEND_ZERO),
	blendStateADst(R_BlendFunc::BS_BLEND_ZERO),
	// Depth
	writeDepth(true),
	depthTest(true),
	depthFunc(R_DepthFunc::DSS_DEPTHFUNC_LESS_EQUAL),
	// Tesselation shaders
	tessVerticesInPatchCount(1)
{}

void ShaderContext::Apply()
{
	RenderDevice* rdi = &_Render->r;

	rdi->setCullMode(cullMode);
	rdi->setAlphaToCoverage(alphaToCoverage);
	rdi->setBlendModeEx(blendingEnabled, blendStateRGBSrc, blendStateRGBDst, blendStateASrc, blendStateADst);
	rdi->setDepthMask(writeDepth);
	rdi->setDepthTest(depthTest);
	rdi->setDepthFunc(depthFunc);
	rdi->setTessPatchVertices(tessVerticesInPatchCount);
}
