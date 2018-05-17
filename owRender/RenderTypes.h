#pragma once

// ---------------------------------------------------------
// Vertex layout
// ---------------------------------------------------------

struct R_VertexLayoutAttrib
{
	string  semanticName;
	uint32  vbSlot;
	uint32  size;
	uint32  offset;
};

struct R_VertexLayout
{
	uint32              numAttribs;
	R_VertexLayoutAttrib  attribs[16];
};


// ---------------------------------------------------------
// Buffers
// ---------------------------------------------------------

#include "R_Buffer.h"

struct R_VertexBufferSlot
{
	R_VertexBufferSlot() :
		vbObj(nullptr),
        type(R_DataType::T_FLOAT),
		offset(0),
		stride(0),
        needNorm(false)
	{}

	R_VertexBufferSlot(R_Buffer* vbObj, R_DataType type, uint32 offset, uint32 stride, bool needNorm) :
		vbObj(vbObj),
        type(type),
		offset(offset),
		stride(stride),
        needNorm(needNorm)
	{}

	//

	R_Buffer*   vbObj;
    R_DataType  type;
	uint32      offset;
	uint32      stride;
    bool        needNorm;
};

#include "R_GeometryInfo.h"

struct R_ShaderStorage
{
	R_ShaderStorage(uint8 targetSlot, uint32 glObj) :
		oglObject(glObj),
		slot(targetSlot)
	{}

	//

	uint32 oglObject;
	uint8 slot;
};



// ---------------------------------------------------------
// Textures
// ---------------------------------------------------------

#include "R_Texture.h"

struct R_TexSlot
{
	R_TexSlot() :
		texObj(nullptr),
		samplerState(0),
		usage(0)
	{}
	R_TexSlot(R_Texture* texObj, uint32 samplerState, uint32 usage) :
		texObj(texObj),
		samplerState(samplerState),
		usage(usage)
	{}

	//

    R_Texture*  texObj;
	uint32      samplerState;
	uint32      usage;
};

#include "R_TextureBuffer.h"


// ---------------------------------------------------------
// Shaders
// ---------------------------------------------------------

struct R_InputLayout
{
	R_InputLayout() : valid(false)
	{
		memset(attribIndices, 0, sizeof(attribIndices));
	}

	//

	bool  valid;
	int8  attribIndices[16];
};

#include "R_Shader.h"



// ---------------------------------------------------------
// Render buffers
// ---------------------------------------------------------

#include "R_RenderBuffer.h"