#pragma once

#include "RenderEnums.h"

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


#include "R_Buffer.h"

#include "R_GeometryInfo.h"

struct R_ShaderStorage
{
	R_ShaderStorage(uint8 _targetSlot, uint32 _GLObj) :
		oglObject(_GLObj),
		slot(_targetSlot)
	{}

	//

	uint32 oglObject;
	uint8 slot;
};

#include "R_Texture.h"

struct R_TexSlot
{
	R_TexSlot() :
		m_Texture(nullptr),
		m_SamplerState(0),
		usage(0)
	{}
	R_TexSlot(R_Texture* _texture, uint32 _samplerState, uint32 _usage) :
		m_Texture(_texture),
		m_SamplerState(_samplerState),
		usage(_usage)
	{}

	//

    R_Texture*  m_Texture;
	uint32      m_SamplerState;
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

#include "R_RenderBuffer.h"