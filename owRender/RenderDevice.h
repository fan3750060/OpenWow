#pragma once

const uint32 MaxNumVertexLayouts = 64;
const uint32 MaxComputeImages = 8;

#include "GPUTimer.h"
#include "RenderTypes.h"

class RenderDevice
{
	friend R_Buffer;
	friend R_TextureBuffer;
	friend R_GeometryInfo;
	friend R_RenderBuffer;
	friend R_Shader;
	friend R_Texture;
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

	// Textures
	R_Texture* createTexture(R_TextureTypes::List type, int width, int height, int depth, R_TextureFormats::List format, bool hasMips, bool genMips, bool compress, bool sRGB);
	uint32 getTextureMem() const { return m_TextureMem; }

	// Shaders
	R_Shader* createShader(const char *vertexShaderSrc, const char *fragmentShaderSrc, const char *geometryShaderSrc, const char *tessControlShaderSrc, const char *tessEvaluationShaderSrc, const char *computeShaderSrc);
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
	void setViewport(int x, int y, int width, int height)
	{
		m_ViewportX = x;
		m_ViewportY = y;
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		_pendingMask |= PM_VIEWPORT;
	}
	void setScissorRect(int x, int y, int width, int height)
	{
		_scX = x;
		_scY = y;
		_scWidth = width;
		_scHeight = height;
		_pendingMask |= PM_SCISSOR;
	}
	void setGeometry(R_GeometryInfo* geoIndex)
	{
		m_CurrentGeometry = geoIndex;
		_pendingMask |= PM_GEOMETRY;
	}
	void setTexture(uint32 slot, R_Texture* texObj, uint16 samplerState, uint16 usage)
	{
		assert1(slot < 16);
		m_TextureSlot[slot] = R_TexSlot(texObj, samplerState, usage);
		_pendingMask |= PM_TEXTURES;
	}
	void setMemoryBarrier(R_DrawBarriers barrier)
	{
		_memBarriers = barrier;
		_pendingMask |= PM_BARRIER;
	}
	void setStorageBuffer(uint8 slot, R_Buffer* bufObj)
	{
		assert1(slot < _maxComputeBufferAttachments && m_StorageBufs.size() < _maxComputeBufferAttachments);

		R_Buffer* buf = bufObj;
		m_StorageBufs.push_back(R_ShaderStorage(slot, buf->m_GLObj));

		_pendingMask |= PM_COMPUTE;
	}

	// Render states
	void setColorWriteMask(bool enabled)
	{
		m_NewRasterState.renderTargetWriteMask = enabled;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getColorWriteMask(bool &enabled) const
	{
		enabled = m_NewRasterState.renderTargetWriteMask;
	}

	void setFillMode(R_FillMode fillMode)
	{
		m_NewRasterState.fillMode = fillMode;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getFillMode(R_FillMode &fillMode) const
	{
		fillMode = (R_FillMode)m_NewRasterState.fillMode;
	}

	void setCullMode(R_CullMode cullMode)
	{
		m_NewRasterState.cullMode = cullMode;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getCullMode(R_CullMode &cullMode) const
	{
		cullMode = (R_CullMode)m_NewRasterState.cullMode;
	}

	void setScissorTest(bool enabled)
	{
		m_NewRasterState.scissorEnable = enabled;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getScissorTest(bool &enabled) const
	{
		enabled = m_NewRasterState.scissorEnable;
	}

	void setMulisampling(bool enabled)
	{
		m_NewRasterState.multisampleEnable = enabled;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getMulisampling(bool &enabled) const
	{
		enabled = m_NewRasterState.multisampleEnable;
	}

	void setAlphaToCoverage(bool enabled)
	{
		m_NewBlendState.alphaToCoverageEnable = enabled;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getAlphaToCoverage(bool &enabled) const
	{
		enabled = m_NewBlendState.alphaToCoverageEnable;
	}

	void setBlendMode(bool enabled, R_BlendFunc srcRGBBlendFunc = BS_BLEND_ZERO, R_BlendFunc destRGBBlendFunc = BS_BLEND_ZERO)
	{
		m_NewBlendState.blendEnable = enabled;

		m_NewBlendState.srcRGBBlendFunc = srcRGBBlendFunc;
		m_NewBlendState.destRGBBlendFunc = destRGBBlendFunc;
		m_NewBlendState.srcABlendFunc = srcRGBBlendFunc;
		m_NewBlendState.destABlendFunc = destRGBBlendFunc;

		_pendingMask |= PM_RENDERSTATES;
	}
	void setBlendModeEx(bool enabled, R_BlendFunc srcRGBBlendFunc = BS_BLEND_ZERO, R_BlendFunc destRGBBlendFunc = BS_BLEND_ZERO, R_BlendFunc srcABlendFunc = BS_BLEND_ZERO, R_BlendFunc destABlendFunc = BS_BLEND_ZERO)
	{
		m_NewBlendState.blendEnable = enabled;

		m_NewBlendState.srcRGBBlendFunc = srcRGBBlendFunc;
		m_NewBlendState.destRGBBlendFunc = destRGBBlendFunc;
		m_NewBlendState.srcABlendFunc = srcABlendFunc;
		m_NewBlendState.destABlendFunc = destABlendFunc;

		_pendingMask |= PM_RENDERSTATES;
	}
	void getBlendMode(bool &enabled, R_BlendFunc &srcRGBBlendFunc, R_BlendFunc &destRGBBlendFunc) const
	{
		enabled = m_NewBlendState.blendEnable;
		srcRGBBlendFunc = (R_BlendFunc)m_NewBlendState.srcRGBBlendFunc;
		destRGBBlendFunc = (R_BlendFunc)m_NewBlendState.destRGBBlendFunc;
	}

	void setDepthMask(bool enabled)
	{
		m_NewDepthStencilState.depthWriteMask = enabled;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getDepthMask(bool &enabled) const
	{
		enabled = m_NewDepthStencilState.depthWriteMask;
	}

	void setDepthTest(bool enabled)
	{
		m_NewDepthStencilState.depthEnable = enabled;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getDepthTest(bool &enabled) const
	{
		enabled = m_NewDepthStencilState.depthEnable;
	}

	void setDepthFunc(R_DepthFunc depthFunc)
	{
		m_NewDepthStencilState.depthFunc = depthFunc;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getDepthFunc(R_DepthFunc &depthFunc) const
	{
		depthFunc = (R_DepthFunc)m_NewDepthStencilState.depthFunc;
	}

	void setTessPatchVertices(uint16 verts)
	{
		_tessPatchVerts = verts;
		_pendingMask |= PM_RENDERSTATES;
	}
#pragma endregion

private:
	bool commitStates(uint32 filter = 0xFFFFFFFF);
	void resetStates();

public: // Draw calls and clears
	void clear(uint32 flags = CLR_COLOR_RT0 | CLR_COLOR_RT1 | CLR_COLOR_RT2 | CLR_COLOR_RT3 | CLR_DEPTH, float* colorRGBA = 0x0, float depth = 1.0f);
	void draw(R_PrimitiveType primType, uint32 firstVert, uint32 numVerts);
	void drawIndexed(R_PrimitiveType primType, uint32 firstIndex, uint32 numIndices, uint32 firstVert, uint32 numVerts, bool _softReset = true);
	void checkError();

public:
	bool applyVertexLayout(R_GeometryInfo &geo);
	void applySamplerState(R_Texture* tex);
	void applyRenderStates();

private:
	R_VertexLayout				m_VertexLayouts[MaxNumVertexLayouts];


	vector<R_ShaderStorage>		m_StorageBufs;
	R_TexSlot					m_TextureSlot[16];
	R_RasterState				m_CurRasterState, m_NewRasterState;
	R_BlendState				m_CurBlendState, m_NewBlendState;
	R_DepthStencilState			m_CurDepthStencilState, m_NewDepthStencilState;
	R_DrawBarriers				_memBarriers;

	R_IndexFormat				m_IndexBufferFormat;
	uint32						_activeVertexAttribsMask;

	uint16						_lastTessPatchVertsValue;
	uint16						_maxComputeBufferAttachments;

	bool						_doubleBuffered;

	// DEFAULT

	// 8 ssbo

	string					    m_ShaderLog;
	uint32						m_DepthFormat;
	int							m_ViewportX, m_ViewportY, m_ViewportWidth, m_ViewportHeight;
	int							_scX, _scY, _scWidth, _scHeight;
	int							_fbWidth, _fbHeight;
	R_RenderBuffer*				_curRendBuf;
	int							_outputBufferIndex;  // Left and right eye for stereo rendering
	uint32						m_TextureMem, m_BufferMem;

	uint32                      _numVertexLayouts;

	R_Shader*					_curShaderId;
	uint32						_pendingMask;
	R_GeometryInfo*				m_DefaultGeometry;
	R_GeometryInfo*				m_CurrentGeometry;
	uint32						_maxTexSlots; // specified in inherited render devices

	uint32						_tessPatchVerts; // number of vertices in patch. Used for tesselation.

	int							m_DefaultFBO;
	bool                        m_IsDefaultFBOMultisampled;

	CGroupRenderCaps&			m_DeviceCapsSettings;
	CGroupOpenGL&				m_OpenGLSettings;
};
