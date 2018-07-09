#pragma once

#include "RenderEnums.h"

struct ShaderContext
{
	ShaderContext();
	
	void Apply();

	// Culling
	R_CullMode		cullMode;
	// Alpha to coverage
	bool			alphaToCoverage;
	// Blend
	bool			blendingEnabled;
	R_BlendFunc		blendStateRGBSrc;
	R_BlendFunc		blendStateRGBDst;
	R_BlendFunc		blendStateASrc;
	R_BlendFunc		blendStateADst;
	// Depth
	bool			writeDepth;
	bool			depthTest;
	R_DepthFunc		depthFunc;
	// Tesselation shaders
	uint16			tessVerticesInPatchCount;

	// Shaders
	//vector< ShaderCombination >  shaderCombs;
};