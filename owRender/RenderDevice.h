#pragma once

const uint32 MaxNumVertexLayouts = 64;
const uint32 MaxComputeImages = 8;

#include "GPUTimer.h"
#include "RenderTypes.h"
#include "RenderStates.h"



class RenderDevice
{
	friend R_Buffer;
	friend R_TextureBuffer;
	friend R_GeometryInfo;
	friend R_RenderBuffer;
	friend R_Shader;
	friend R_Texture;
	friend RenderState;
public:
	RenderDevice();
	~RenderDevice();

	bool init(IOpenGLAdapter* _adapter);

	// Vertex layouts
	uint32 registerVertexLayout(uint32 numAttribs, R_VertexLayoutAttrib *attribs);

	// Rendering
	void beginRendering();

	// Geometry
	R_GeometryInfo* beginCreatingGeometry(uint32 _vertexLayout);

	// Buffers
	R_Buffer* createVertexBuffer(uint32 size, const void *data, bool _isDynamic = true);
	R_Buffer* createIndexBuffer(uint32 size, const void *data, bool _isDynamic = true);
	R_Buffer* createShaderStorageBuffer(uint32 size, const void *data, bool _isDynamic = true);
	R_TextureBuffer* createTextureBuffer(R_TextureFormats::List format, uint32 bufSize, const void *data, bool _isDynamic = true);
	uint32 getBufferMem() const { return m_BufferMem; }
	string getBufferMemStr() const { return convertToString(m_BufferMem); }

	// Textures
	R_Texture* createTexture(R_TextureTypes::List type, int width, int height, int depth, R_TextureFormats::List format, bool hasMips, bool genMips, bool compress, bool sRGB);
	uint32 getTextureMem() const { return m_TextureMem; }
	string getTextureMemStr() const { return convertToString(m_TextureMem); }

	// Shaders
	R_Shader* createShader(cstring _name, const char *vertexShaderSrc, const char *fragmentShaderSrc, const char *geometryShaderSrc, const char *tessControlShaderSrc, const char *tessEvaluationShaderSrc, const char *computeShaderSrc);
	string getShaderLog() const { return m_ShaderLog; }

	// Renderbuffers
	R_RenderBuffer* createRenderBuffer(uint32 width, uint32 height, R_TextureFormats::List format, bool depth, uint32 numColBufs, uint32 samples);
	R_RenderBuffer* createRenderBuffer(vec2 _size, R_TextureFormats::List format, bool depth, uint32 numColBufs, uint32 samples);

	// Queries
	uint32 createOcclusionQuery();
	void destroyQuery(uint32 queryObj);
	void beginQuery(uint32 queryObj);
	void endQuery(uint32 queryObj);
	uint32 getQueryResult(uint32 queryObj);

	// Render Device dependent GPU Timer
	GPUTimer* createGPUTimer() { return new GPUTimer(); }

#pragma region Commands
	RenderState* getState() { return &m_State; }

	void setViewport(int x, int y, int width, int height)
	{
		m_State.setViewport(x, y, width, height);
	}
	void setScissorRect(int x, int y, int width, int height)
	{
		m_State.setScissorRect(x, y, width, height);
	}
	void setGeometry(R_GeometryInfo* geoIndex)
	{
		m_State.setGeometry(geoIndex);
	}
	void setTexture(uint32 slot, R_Texture* texObj, uint16 samplerState, uint16 usage)
	{
		m_State.setTexture(slot, texObj, samplerState, usage);
	}
	void setMemoryBarrier(R_DrawBarriers barrier)
	{
		m_State.setMemoryBarrier(barrier);
	}
	void setStorageBuffer(uint8 slot, R_Buffer* bufObj)
	{
		m_State.setStorageBuffer(slot, bufObj);
	}

	// Render states
	void setColorWriteMask(bool enabled)
	{
		m_State.setColorWriteMask(enabled);
	}
	void getColorWriteMask(bool &enabled) const
	{
		//enabled = m_NewRasterState.renderTargetWriteMask;
	}

	void setFillMode(R_FillMode fillMode)
	{
		m_State.setFillMode(fillMode);
	}
	void getFillMode(R_FillMode &fillMode) const
	{
		//fillMode = (R_FillMode)m_NewRasterState.fillMode;
		fail1();
	}

	void setCullMode(R_CullMode cullMode)
	{
		m_State.setCullMode(cullMode);
	}
	void getCullMode(R_CullMode &cullMode) const
	{
		//cullMode = (R_CullMode)m_NewRasterState.cullMode;
		fail1();
	}

	void setScissorTest(bool enabled)
	{
		m_State.setScissorTest(enabled);
	}
	void getScissorTest(bool &enabled) const
	{
		//enabled = m_NewRasterState.scissorEnable;
		fail1();
	}

	void setMulisampling(bool enabled)
	{
		m_State.setMulisampling(enabled);
	}
	void getMulisampling(bool &enabled) const
	{
		//enabled = m_NewRasterState.multisampleEnable;
		fail1();
	}

	void setAlphaToCoverage(bool enabled)
	{
		m_State.setAlphaToCoverage(enabled);
	}
	void getAlphaToCoverage(bool &enabled) const
	{
		//enabled = m_NewBlendState.alphaToCoverageEnable;
		fail1();
	}

	void setBlendMode(bool enabled, R_BlendFunc srcBlendFunc = BS_BLEND_ZERO, R_BlendFunc dstBlendFunc = BS_BLEND_ZERO)
	{
		m_State.setBlendMode(enabled, srcBlendFunc, dstBlendFunc);
	}
	void getBlendMode(bool &enabled, R_BlendFunc &srcRGBBlendFunc, R_BlendFunc &dstRGBBlendFunc) const
	{
		//enabled = m_NewBlendState.blendEnable;
		//srcRGBBlendFunc = (R_BlendFunc)m_NewBlendState.srcRGBBlendFunc;
		//dstRGBBlendFunc = (R_BlendFunc)m_NewBlendState.dstRGBBlendFunc;
		fail1();
	}
	void setBlendModeEx(bool enabled, R_BlendFunc srcRGBBlendFunc = BS_BLEND_ZERO, R_BlendFunc dstRGBBlendFunc = BS_BLEND_ZERO, R_BlendFunc srcABlendFunc = BS_BLEND_ZERO, R_BlendFunc dstABlendFunc = BS_BLEND_ZERO)
	{
		m_State.setBlendModeEx(enabled, srcRGBBlendFunc, dstRGBBlendFunc, srcABlendFunc, dstABlendFunc);
	}
	void getBlendModeEx(bool &enabled, R_BlendFunc &srcRGBBlendFunc, R_BlendFunc &dstRGBBlendFunc, R_BlendFunc &srcABlendFunc, R_BlendFunc &dstABlendFunc) const
	{
		//enabled = m_NewBlendState.blendEnable;
		//srcRGBBlendFunc = (R_BlendFunc)m_NewBlendState.srcRGBBlendFunc;
		//dstRGBBlendFunc = (R_BlendFunc)m_NewBlendState.dstRGBBlendFunc;
		//srcABlendFunc = (R_BlendFunc)m_NewBlendState.srcABlendFunc;
		//dstABlendFunc = (R_BlendFunc)m_NewBlendState.dstABlendFunc;
		fail1();
	}

	void setDepthMask(bool enabled)
	{
		m_State.setDepthMask(enabled);
	}
	void getDepthMask(bool &enabled) const
	{
		//enabled = m_NewDepthStencilState.depthWriteMask;
		fail1();
	}

	void setDepthTest(bool enabled)
	{
		m_State.setDepthTest(enabled);
	}
	void getDepthTest(bool &enabled) const
	{
		//enabled = m_NewDepthStencilState.depthEnable;
		fail1();
	}

	void setDepthFunc(R_DepthFunc depthFunc)
	{
		m_State.setDepthFunc(depthFunc);
	}
	void getDepthFunc(R_DepthFunc &depthFunc) const
	{
		//depthFunc = (R_DepthFunc)m_NewDepthStencilState.depthFunc;
		fail1();
	}

	void setTessPatchVertices(uint16 verts)
	{
		m_State.setTessPatchVertices(verts);
	}
#pragma endregion

private:
	void commitStates(RenderState* _newStsate, uint32 filter = 0xFFFFFFFF);
	void resetStates();

public: // Draw calls and clears
	void clear(uint32 flags = CLR_COLOR_RT0 | CLR_COLOR_RT1 | CLR_COLOR_RT2 | CLR_COLOR_RT3 | CLR_DEPTH, float* colorRGBA = 0x0, float depth = 1.0f);
	void draw(R_PrimitiveType primType, uint32 firstVert, uint32 numVerts, RenderState* _state = nullptr);
	void drawIndexed(R_PrimitiveType primType, uint32 firstIndex, uint32 numIndices, uint32 firstVert, uint32 numVerts, RenderState* _state = nullptr, bool _softReset = true);
	void checkError();

public:
	void applySamplerState(R_Texture* tex);
	void applyRenderStates(RenderState* _state);

private:
	R_VertexLayout				m_VertexLayouts[MaxNumVertexLayouts];

	uint32						_activeVertexAttribsMask;

	
	uint16						_maxComputeBufferAttachments;

	bool						_doubleBuffered;

	string					    m_ShaderLog;
	uint32						m_DepthFormat;

	
	int							_outputBufferIndex;  // Left and right eye for stereo rendering
	uint32						m_TextureMem, m_BufferMem;

	uint32                      _numVertexLayouts;

	


	SmartGeomPtr				m_DefaultGeometry;


	//------------------------------------------------------------------------------------
	// FBO
	int							m_RBWidth, m_RBHeight;
	R_RenderBuffer*				m_RBCurrent;
	int							m_RBDefault;
	bool                        m_IsDefaultFBOMultisampled;

	//------------------------------------------------------------------------------------
	// Render state

	R_RasterState				m_CurRasterState;
	R_BlendState				m_CurBlendState;
	R_DepthStencilState			m_CurDepthStencilState;

	uint16						m_CurrTessPatchVertsValue;

	RenderState					m_State;

	//------------------------------------------------------------------------------------
	// Settings
	CGroupRenderCaps&			m_DeviceCapsSettings;
	CGroupOpenGL&				m_OpenGLSettings;
};
