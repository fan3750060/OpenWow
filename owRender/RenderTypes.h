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

struct R_Buffer
{
	R_Buffer() :
		type(0),
		glObj(0),
		size(0),
        isDynamic(true),
		geometryRefCount(0)
	{}

	//

	uint32  type;
	uint32  glObj;
	uint32  size;
    bool    isDynamic;
	int		geometryRefCount;
};

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

struct R_GeometryInfo
{
	R_GeometryInfo() :
		vao(0),
		indexBuf(nullptr),
		layout(0),
		indexBuf32Bit(false),
		atrribsBinded(false)
	{}

	//

	vector< R_VertexBufferSlot > vertexBufInfo;
	uint32 vao;
	R_Buffer* indexBuf;
	uint32 layout;
	bool indexBuf32Bit;
	bool atrribsBinded;
};

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

class R_Texture : public RefItem
{
public:
	R_Texture() :
		glObj(0),
		glFmt(0),
		type(0),
		format(R_TextureFormats::Unknown),
		width(0),
		height(0),
		depth(0),
		memSize(0),
		samplerState(0),
		sRGB(false),
		hasMips(false),
		genMips(false)
	{}

	//

	uint32                  glObj;
	uint32                  glFmt;
	int                     type;
	R_TextureFormats::List  format;
	int                     width, height, depth;
	int                     memSize;
	uint32                  samplerState;
	bool                    sRGB;
	bool                    hasMips, genMips;

    vec2 GetSize()
    {
        return vec2(static_cast<float>(width), static_cast<float>(height));
    }
};

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
	uint32  samplerState;
	uint32  usage;
};

struct R_TextureBuffer
{
	R_TextureBuffer() :
		bufObj(nullptr),
		glFmt(0),
		glTexID(0)
	{}

	//

	R_Buffer* bufObj;
	uint32    glFmt;
	uint32	  glTexID;
};



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

struct R_Shader
{
	R_Shader() :
		oglProgramObj(0)
	{}

	//

	uint32			oglProgramObj;
	R_InputLayout	inputLayouts[MaxNumVertexLayouts];
};



// ---------------------------------------------------------
// Render buffers
// ---------------------------------------------------------

struct R_RenderBuffer
{
	static const uint32 MaxColorAttachmentCount = 4;

	//

	R_RenderBuffer() :
		fbo(0),
		fboMS(0),
		width(0),
		height(0),
		depthTex(0),
		depthBuf(0),
		samples(0)
	{
		for (uint32 i = 0; i < MaxColorAttachmentCount; ++i)
		{
            colTexs[i] = nullptr;
            colBufs[i] = 0;
		}
	}

	//

	uint32  fbo, fboMS;
	uint32  width, height;
	uint32  samples;

    R_Texture*  depthTex;
    R_Texture*  colTexs[MaxColorAttachmentCount];
    uint32      depthBuf;
    uint32      colBufs[MaxColorAttachmentCount];  // Used for multisampling
};