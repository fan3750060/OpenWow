#include "stdafx.h"

// General
#include "RenderDevice.h"

// Additional
#include "OpenGL.h"

#define CHECK_GL_ERROR checkError();

// Bindings for RDI types to GL


static const uint32 indexFormats[2] = {GL_UNSIGNED_SHORT, GL_UNSIGNED_INT};

static const uint32 primitiveTypes[5] = {GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_LINES, GL_POINTS, GL_PATCHES};

static const uint32 memoryBarrierType[3] = {GL_BUFFER_UPDATE_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT, GL_ELEMENT_ARRAY_BARRIER_BIT, GL_SHADER_IMAGE_ACCESS_BARRIER_BIT};

static const uint32 oglBlendFuncs[10] = {
	GL_ZERO, 
	GL_ONE, 

	GL_SRC_ALPHA, 
	GL_ONE_MINUS_SRC_ALPHA, 
	GL_DST_ALPHA, 
	GL_ONE_MINUS_DST_ALPHA, 

	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR, 
	GL_ONE_MINUS_DST_COLOR
};

void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	//if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	//  Dynamic buff	pixel sync
	if (id == 131185 || id == 131154) return;


	Log::Error("---------------");
	Log::Error("OpenGL Debug message (%d): [%s]", id, message);

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             Log::Error("Source: OpenGL API"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   Log::Error("Source: Window System API"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: Log::Error("Source: Shader Compiler"); break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     Log::Error("Source: Third Party"); break;
		case GL_DEBUG_SOURCE_APPLICATION:     Log::Error("Source: Application"); break;
		case GL_DEBUG_SOURCE_OTHER:           Log::Error("Source: Other"); break;
	}

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               Log::Error("Type: Error"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: Log::Error("Type: Deprecated Behaviour"); break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  Log::Error("Type: Undefined Behaviour"); break;
		case GL_DEBUG_TYPE_PORTABILITY:         Log::Error("Type: Portability"); break;
		case GL_DEBUG_TYPE_PERFORMANCE:         Log::Error("Type: Performance"); break;
		case GL_DEBUG_TYPE_MARKER:              Log::Error("Type: Marker"); break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          Log::Error("Type: Push Group"); break;
		case GL_DEBUG_TYPE_POP_GROUP:           Log::Error("Type: Pop Group"); break;
		case GL_DEBUG_TYPE_OTHER:               Log::Error("Type: Other"); break;
	}

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         Log::Error("Severity: high"); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       Log::Error("Severity: medium"); break;
		case GL_DEBUG_SEVERITY_LOW:          Log::Error("Severity: low"); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: Log::Error("Severity: notification"); break;
	}


	//system("pause");
	//Log::Exit(-1);
}



// =================================================================================================
// Common
// =================================================================================================

RenderDevice::RenderDevice() :
	m_DeviceCapsSettings(GetSettingsGroup<CGroupRenderCaps>()),
	m_OpenGLSettings(GetSettingsGroup<CGroupOpenGL>())
{
	_numVertexLayouts = 0;

	m_ViewportX = 0; m_ViewportY = 0; m_ViewportWidth = 320; m_ViewportHeight = 240;
	_scX = 0; _scY = 0; _scWidth = 320; _scHeight = 240;

	_curShaderId = 0;
	_curRendBuf = 0; _outputBufferIndex = 0;
	m_TextureMem = 0; m_BufferMem = 0;
	m_CurRasterState.hash = m_NewRasterState.hash = 0;
	_curBlendState.hash = _newBlendState.hash = 0;
	_curDepthStencilState.hash = _newDepthStencilState.hash = 0;
	m_DefaultFBO = 0;
	M_DefaultFBOMultisampled = false;
	m_IsIndexFormat32 = false;
	_activeVertexAttribsMask = 0;
	_pendingMask = 0;
	_tessPatchVerts = _lastTessPatchVertsValue = 0;
	_memBarriers = NotSet;

	_maxComputeBufferAttachments = 8;
	_storageBufs.reserve(_maxComputeBufferAttachments);

	_maxTexSlots = 32; // texture units per stage

	_doubleBuffered = false;

	// add default geometry for resetting
    _defGeometry = new R_GeometryInfo(this);
    _defGeometry->atrribsBinded = true;

    _curGeometryIndex = _defGeometry;
}

RenderDevice::~RenderDevice()
{}

bool RenderDevice::init()
{
	bool failed = false;

	char* vendor = (char*)glGetString(GL_VENDOR);
	char* renderer = (char*)glGetString(GL_RENDERER);
	char* version = (char*)glGetString(GL_VERSION);

	if (!version || !renderer || !vendor)
	{
		Log::Error("OpenGL not initialized. Make sure you have a valid OpenGL context");
		return false;
	}

	Log::Info("Initializing GL4 backend.");
	Log::Info("OpenGL driver[%s] by[%s] on[%s]", version, vendor, renderer);

	// Init extensions
	if (!initOpenGLExtensions())
	{
		Log::Error("Could not find all required OpenGL function entry points");
		failed = true;
	}

	// Check that OpenGL 3.3 is available
	if (m_OpenGLSettings.majorVersion * 10 + m_OpenGLSettings.minorVersion < 33)
	{
		Log::Error("OpenGL 3.3 not available");
		failed = true;
	}

	// Check that required extensions are supported
	if (!m_OpenGLSettings.EXT_texture_filter_anisotropic)
	{
		Log::Error("Extension EXT_texture_filter_anisotropic not supported");
		failed = true;
	}
	if (!m_OpenGLSettings.EXT_texture_compression_s3tc)
	{
		Log::Error("Extension EXT_texture_compression_s3tc not supported");
		failed = true;
	}
	if (!m_OpenGLSettings.EXT_texture_sRGB)
	{
		Log::Error("Extension EXT_texture_sRGB not supported");
		failed = true;
	}

	if (failed)
	{
		Log::Error("Failed to init renderer backend (OpenGL %d.%d), retrying with legacy OpenGL 2.1 backend", m_OpenGLSettings.majorVersion, m_OpenGLSettings.minorVersion);
		return false;
	}

	// Debug
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);

	// Set capabilities
	m_DeviceCapsSettings.texFloat = true;
	m_DeviceCapsSettings.texNPOT = true;
	m_DeviceCapsSettings.rtMultisampling = true;
	m_DeviceCapsSettings.geometryShaders = true;
	m_DeviceCapsSettings.tesselation = m_OpenGLSettings.majorVersion >= 4 && m_OpenGLSettings.minorVersion >= 1;
	m_DeviceCapsSettings.computeShaders = m_OpenGLSettings.majorVersion >= 4 && m_OpenGLSettings.minorVersion >= 3;
	m_DeviceCapsSettings.instancing = true;
	m_DeviceCapsSettings.maxJointCount = 75; // currently, will be changed soon
	m_DeviceCapsSettings.maxTexUnitCount = 96; // for most modern hardware it is 192 (GeForce 400+, Radeon 7000+, Intel 4000+). Although 96 should probably be enough.

	// Find maximum number of storage buffers in compute shader
	glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, (GLint *)&_maxComputeBufferAttachments);

	// Init states before creating test render buffer, to ensure binding the current FBO again
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		GLint value;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &value);
		M_DefaultFBOMultisampled = value > 0;

		GLboolean doubleBuffered;
		glGetBooleanv(GL_DOUBLEBUFFER, &doubleBuffered);
		_doubleBuffered = doubleBuffered != 0;

		// Get the currently bound frame buffer object to avoid reset to invalid FBO
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_DefaultFBO);
	}

	// Find supported depth format (some old ATI cards only support 16 bit depth for FBOs)
	m_DepthFormat = GL_DEPTH_COMPONENT24;
	R_RenderBuffer* testBuf = createRenderBuffer(32, 32, R_TextureFormats::RGBA8, true, 1, 0);
	if (testBuf == nullptr)
	{
		m_DepthFormat = GL_DEPTH_COMPONENT16;
		Log::Warn("Render target depth precision limited to 16 bit");
	}
	else
	{
		testBuf->destroyRenderBuffer();
	}

	resetStates();

    return true;
}


// Vertex layouts

uint32 RenderDevice::registerVertexLayout(uint32 numAttribs, R_VertexLayoutAttrib *attribs)
{
	if (_numVertexLayouts == MaxNumVertexLayouts)
	{
		return 0;
	}

	m_VertexLayouts[_numVertexLayouts].numAttribs = numAttribs;

	for (uint32 i = 0; i < numAttribs; ++i)
	{
		m_VertexLayouts[_numVertexLayouts].attribs[i] = attribs[i];
	}

	return ++_numVertexLayouts;
}

void RenderDevice::beginRendering()
{
	CHECK_GL_ERROR

	//	Get the currently bound frame buffer object. 
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_DefaultFBO);
	resetStates();
}

// Geometry

R_GeometryInfo* RenderDevice::beginCreatingGeometry(uint32 vlObj)
{
	R_GeometryInfo* vao = new R_GeometryInfo(this);
	vao->layout = vlObj;

	uint32 vaoID;
	glGenVertexArrays(1, &vaoID);
	vao->vao = vaoID;

	return vao;
}

// Buffers

R_Buffer* RenderDevice::createVertexBuffer(uint32 size, const void *data, bool _isDynamic)
{
	R_Buffer* buf = new R_Buffer(this);
	buf->createVertexBuffer(size, data, _isDynamic);
	return buf;
}

R_Buffer* RenderDevice::createIndexBuffer(uint32 size, const void *data, bool _isDynamic)
{
	R_Buffer* buf = new R_Buffer(this);
	buf->createIndexBuffer(size, data, _isDynamic);
	return buf;
}

R_Buffer* RenderDevice::createShaderStorageBuffer(uint32 size, const void *data, bool _isDynamic)
{
	if (m_DeviceCapsSettings.computeShaders)
	{
		R_Buffer* buf = new R_Buffer(this);
		buf->createShaderStorageBuffer(size, data, _isDynamic);
		return buf;
	}
	else
	{
		Log::Error("Shader storage buffers are not supported on this OpenGL 4 device.");

		return 0;
	}
}

R_TextureBuffer* RenderDevice::createTextureBuffer(R_TextureFormats::List format, uint32 bufSize, const void *data, bool _isDynamic)
{
	R_TextureBuffer* buf = new R_TextureBuffer(this);
	buf->createTextureBuffer(format, bufSize, data, _isDynamic);
	return buf;
}

// Textures

R_Texture* RenderDevice::createTexture(R_TextureTypes::List type, int width, int height, int depth, R_TextureFormats::List format, bool hasMips, bool genMips, bool compress, bool sRGB)
{
    R_Texture* tex = new R_Texture(this);
	tex->createTexture(type, width, height, depth, format, hasMips, genMips, compress, sRGB);
	return tex;
}

// Shaders

R_Shader* RenderDevice::createShader(const char * vertexShaderSrc, const char * fragmentShaderSrc, const char * geometryShaderSrc, const char * tessControlShaderSrc, const char * tessEvaluationShaderSrc, const char * computeShaderSrc)
{
	R_Shader* shader = new R_Shader(this);
	shader->createShader(vertexShaderSrc, fragmentShaderSrc, geometryShaderSrc, tessControlShaderSrc, tessEvaluationShaderSrc, computeShaderSrc);
	return shader;
}

// Renderbuffers

R_RenderBuffer* RenderDevice::createRenderBuffer(uint32 width, uint32 height, R_TextureFormats::List format, bool depth, uint32 numColBufs, uint32 samples)
{
	R_RenderBuffer* rb = new R_RenderBuffer(this);
	rb->createRenderBuffer(width, height, format, depth, numColBufs, samples);
	return rb;
}

// Queries

uint32 RenderDevice::createOcclusionQuery()
{
	uint32 queryObj;
	glGenQueries(1, &queryObj);
	return queryObj;
}

void RenderDevice::destroyQuery(uint32 queryObj)
{
	if (queryObj == 0) return;

	glDeleteQueries(1, &queryObj);
}

void RenderDevice::beginQuery(uint32 queryObj)
{
	glBeginQuery(GL_SAMPLES_PASSED, queryObj);
}

void RenderDevice::endQuery(uint32 /*queryObj*/)
{
	glEndQuery(GL_SAMPLES_PASSED);
}

uint32 RenderDevice::getQueryResult(uint32 queryObj)
{
	uint32 samples = 0;
	glGetQueryObjectuiv(queryObj, GL_QUERY_RESULT, &samples);
	return samples;
}

// Public state management

bool RenderDevice::commitStates(uint32 filter)
{
	if (_pendingMask & filter)
	{
		uint32 mask = _pendingMask & filter;

		// Set viewport
		if (mask & PM_VIEWPORT)
		{
			glViewport(m_ViewportX, m_ViewportY, m_ViewportWidth, m_ViewportHeight);
			_pendingMask &= ~PM_VIEWPORT;
		}

		CHECK_GL_ERROR

		// Update render state
		if (mask & PM_RENDERSTATES)
		{
			applyRenderStates();
			_pendingMask &= ~PM_RENDERSTATES;
		}

		CHECK_GL_ERROR

		// Set scissor rect
		if (mask & PM_SCISSOR)
		{
			glScissor(_scX, _scY, _scWidth, _scHeight);
			_pendingMask &= ~PM_SCISSOR;
		}

		CHECK_GL_ERROR

		// Bind m_DiffuseTextures and set sampler state
		if (mask & PM_TEXTURES)
		{
			for (uint32 i = 0; i < 16; i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);

				if (m_TextureSlot[i].usage != R_TextureUsage::Texture && m_TextureSlot[i].texObj != nullptr)
				{
					if (i >= MaxComputeImages)
					{
						continue;
					}

					R_Texture*& tex = m_TextureSlot[i].texObj;
					uint32 access[3] = {GL_READ_ONLY, GL_WRITE_ONLY, GL_READ_WRITE};

					glBindImageTexture(i, tex->glObj, 0, false, 0, access[m_TextureSlot[i].usage - 1], tex->glFmt);
					glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // as image units are different from texture units - clear binded texture units
					glBindTexture(GL_TEXTURE_3D, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				else if (m_TextureSlot[i].texObj != nullptr)
				{
					R_Texture*& tex = m_TextureSlot[i].texObj;
					glBindTexture(tex->type, tex->glObj);

					// Apply sampler state
					if (tex->samplerState != m_TextureSlot[i].samplerState)
					{
						tex->samplerState = m_TextureSlot[i].samplerState;
						applySamplerState(tex);
					}
				}
				else
				{
					glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
					glBindTexture(GL_TEXTURE_3D, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			_pendingMask &= ~PM_TEXTURES;
		}

		CHECK_GL_ERROR

		// Bind vertex buffers
		if (mask & PM_GEOMETRY)
		{
			glBindVertexArray(_curGeometryIndex->vao);

			m_IsIndexFormat32 = _curGeometryIndex->indexBuf32Bit;
			_pendingMask &= ~PM_GEOMETRY;
		}

		CHECK_GL_ERROR

		// Place memory barriers
		if (mask & PM_BARRIER)
		{
			if (_memBarriers != NotSet)
			{
				glMemoryBarrier(memoryBarrierType[(uint32)_memBarriers - 1]);
			}
			_pendingMask &= ~PM_BARRIER;
		}

		CHECK_GL_ERROR

		// Bind storage buffers
		if (mask & PM_COMPUTE)
		{
			for (uint32_t i = 0; i < _storageBufs.size(); ++i)
			{
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _storageBufs[i].slot, _storageBufs[i].oglObject);
			}

			_pendingMask &= ~PM_COMPUTE;
		}

		CHECK_GL_ERROR
	}

	return true;
}

void RenderDevice::resetStates()
{
	CHECK_GL_ERROR

	_curGeometryIndex = _defGeometry;
	m_CurRasterState.hash = 0xFFFFFFFF; m_NewRasterState.hash = 0;
	_curBlendState.hash = 0xFFFFFFFF; _newBlendState.hash = 0;
	_curDepthStencilState.hash = 0xFFFFFFFF; _newDepthStencilState.hash = 0;

	_memBarriers = NotSet;

	for (uint32 i = 0; i < 16; ++i)
	{
		setTexture(i, nullptr, 0, 0);
	}

	_storageBufs.clear();

	setColorWriteMask(true);
	_pendingMask = 0xFFFFFFFF;
	commitStates();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, m_DefaultFBO);
}

// Draw calls and clears

void RenderDevice::clear(uint32 flags, float *colorRGBA, float depth)
{
	uint32 prevBuffers[4] = {0};

	if (_curRendBuf != 0x0)
	{
		if ((flags & CLR_DEPTH) && _curRendBuf->depthTex == 0)
		{
			flags &= ~CLR_DEPTH;
		}

		// Store state of glDrawBuffers
		for (uint32 i = 0; i < 4; ++i)
		{
			glGetIntegerv(GL_DRAW_BUFFER0 + i, (int *)&prevBuffers[i]);
		}

		uint32 buffers[4], cnt = 0;

		if ((flags & CLR_COLOR_RT0) && _curRendBuf->colTexs[0] != 0) buffers[cnt++] = GL_COLOR_ATTACHMENT0;
		if ((flags & CLR_COLOR_RT1) && _curRendBuf->colTexs[1] != 0) buffers[cnt++] = GL_COLOR_ATTACHMENT1;
		if ((flags & CLR_COLOR_RT2) && _curRendBuf->colTexs[2] != 0) buffers[cnt++] = GL_COLOR_ATTACHMENT2;
		if ((flags & CLR_COLOR_RT3) && _curRendBuf->colTexs[3] != 0) buffers[cnt++] = GL_COLOR_ATTACHMENT3;

		if (cnt == 0)
		{
			flags &= ~(CLR_COLOR_RT0 | CLR_COLOR_RT1 | CLR_COLOR_RT2 | CLR_COLOR_RT3);
		}
		else
		{
			glDrawBuffers(cnt, buffers);
		}
	}

	uint32 oglClearMask = 0;

	if (flags & CLR_DEPTH)
	{
		oglClearMask |= GL_DEPTH_BUFFER_BIT;
		glClearDepth(depth);
	}
	if (flags & (CLR_COLOR_RT0 | CLR_COLOR_RT1 | CLR_COLOR_RT2 | CLR_COLOR_RT3))
	{
		oglClearMask |= GL_COLOR_BUFFER_BIT;
		if (colorRGBA)
			glClearColor(colorRGBA[0], colorRGBA[1], colorRGBA[2], colorRGBA[3]);
		else
			glClearColor(0, 0, 0, 0);
	}

	if (oglClearMask)
	{
		commitStates(PM_VIEWPORT | PM_SCISSOR | PM_RENDERSTATES);
		glClear(oglClearMask);
	}

	// Restore state of glDrawBuffers
	if (_curRendBuf != 0x0)
	{
		glDrawBuffers(4, prevBuffers);
	}

	CHECK_GL_ERROR
}

void RenderDevice::draw(R_PrimitiveType primType, uint32 firstVert, uint32 numVerts)
{
	if (commitStates())
	{
        assert1(_curGeometryIndex != nullptr);
		assert1(_curGeometryIndex != _defGeometry);
		glDrawArrays(primitiveTypes[(uint32)primType], firstVert, numVerts);

		CHECK_GL_ERROR
	}
}

void RenderDevice::drawIndexed(R_PrimitiveType primType, uint32 firstIndex, uint32 numIndices, uint32 firstVert, uint32 numVerts, bool _softReset)
{
	if (commitStates())
	{
		firstIndex *= (m_IsIndexFormat32) ? 4u : 2u;

        assert1(_curGeometryIndex != nullptr);
		assert1(_curGeometryIndex != _defGeometry);
		glDrawRangeElements(primitiveTypes[(uint32)primType], firstVert, firstVert + numVerts, numIndices, indexFormats[m_IsIndexFormat32], (char *)0 + firstIndex);
	
		if (_softReset)
		{
			glBindVertexArray(0);
		}

		CHECK_GL_ERROR
	}
}

// Internal state management

void RenderDevice::checkError()
{
	uint32 error = glGetError();
	assert1(error != GL_INVALID_ENUM);
	assert1(error != GL_INVALID_VALUE);
	assert1(error != GL_INVALID_OPERATION);
	assert1(error != GL_OUT_OF_MEMORY);
	assert1(error != GL_STACK_OVERFLOW && error != GL_STACK_UNDERFLOW);
}

bool RenderDevice::applyVertexLayout(R_GeometryInfo& geo)
{
	uint32 newVertexAttribMask = 0;

	if (_curShaderId == 0)
	{
		return false;
	}

	if (geo.layout == 0)
	{
		return false;
	}

	R_VertexLayout &vl = m_VertexLayouts[geo.layout - 1];
	R_InputLayout &inputLayout = _curShaderId->inputLayouts[geo.layout - 1];

	if (!inputLayout.valid)
	{
		return false;
	}

	// Set vertex attrib pointers
	for (uint32 i = 0; i < vl.numAttribs; ++i)
	{
		int8 attribIndex = inputLayout.attribIndices[i];
		if (attribIndex >= 0)
		{
			R_VertexLayoutAttrib& attrib = vl.attribs[i];
			const R_VertexBufferSlot& vbSlot = geo.vertexBufInfo[attrib.vbSlot];

			assert1(geo.vertexBufInfo[attrib.vbSlot].vbObj->glObj != 0 && geo.vertexBufInfo[attrib.vbSlot].vbObj->type == GL_ARRAY_BUFFER);

			glBindBuffer(GL_ARRAY_BUFFER, geo.vertexBufInfo[attrib.vbSlot].vbObj->glObj);
			glVertexAttribPointer(attribIndex, attrib.size, GL_FLOAT, GL_FALSE, vbSlot.stride, (char *)0 + vbSlot.offset + attrib.offset);

			newVertexAttribMask |= 1 << attribIndex;
		}
	}


	for (uint32 i = 0; i < 16; ++i)
	{
		uint32 curBit = 1 << i;
		if ((newVertexAttribMask & curBit) != (_activeVertexAttribsMask & curBit))
		{
			if (newVertexAttribMask & curBit)
			{
				glEnableVertexAttribArray(i);
			}
			else
			{
				glDisableVertexAttribArray(i);
			}
		}
	}
	_activeVertexAttribsMask = newVertexAttribMask;

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

void RenderDevice::applySamplerState(R_Texture* tex)
{
	uint32 state = tex->samplerState;
	uint32 target = tex->type;

	const uint32 magFilters[] = { GL_LINEAR, GL_LINEAR, GL_NEAREST };
	const uint32 minFiltersMips[] = { GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_NEAREST };
	const uint32 maxAniso[] = { 1, 2, 4, 0, 8, 0, 0, 0, 16 };
	const uint32 wrapModes[] = { GL_CLAMP_TO_EDGE, GL_REPEAT, GL_CLAMP_TO_BORDER };

	if (tex->hasMips)
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFiltersMips[(state & SS_FILTER_MASK) >> SS_FILTER_START]);
	else
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, magFilters[(state & SS_FILTER_MASK) >> SS_FILTER_START]);

	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilters[(state & SS_FILTER_MASK) >> SS_FILTER_START]);
	glTexParameteri(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso[(state & SS_ANISO_MASK) >> SS_ANISO_START]);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapModes[(state & SS_ADDRU_MASK) >> SS_ADDRU_START]);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapModes[(state & SS_ADDRV_MASK) >> SS_ADDRV_START]);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapModes[(state & SS_ADDRW_MASK) >> SS_ADDRW_START]);

	if (state & SS_COMP_LEQUAL)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	}
	else
	{
		glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	}
}

void RenderDevice::applyRenderStates()
{
	// Rasterizer state
	if (m_NewRasterState.hash != m_CurRasterState.hash)
	{
		// FILL MODE
		if (m_NewRasterState.fillMode == RS_FILL_SOLID)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		// CULLING
		if (m_NewRasterState.cullMode == RS_CULL_BACK)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		else if (m_NewRasterState.cullMode == RS_CULL_FRONT)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		// SCISSOR
		if (!m_NewRasterState.scissorEnable)
		{
			glDisable(GL_SCISSOR_TEST);
		}
		else
		{
			glEnable(GL_SCISSOR_TEST);
		}

		// MASK
		if (m_NewRasterState.renderTargetWriteMask)
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		}
		else
		{
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		}

		m_CurRasterState.hash = m_NewRasterState.hash;
	}

	// Blend state
	if (_newBlendState.hash != _curBlendState.hash)
	{
		if (!_newBlendState.alphaToCoverageEnable)
		{
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
		else
		{
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}

		if (!_newBlendState.blendEnable)
		{
			glDisable(GL_BLEND);
		}
		else
		{
			glEnable(GL_BLEND);
			glBlendFuncSeparate(oglBlendFuncs[_newBlendState.srcRGBBlendFunc], oglBlendFuncs[_newBlendState.destRGBBlendFunc],
				oglBlendFuncs[_newBlendState.srcABlendFunc], oglBlendFuncs[_newBlendState.destABlendFunc]);
		}

		_curBlendState.hash = _newBlendState.hash;
	}

	// Depth-stencil state
	if (_newDepthStencilState.hash != _curDepthStencilState.hash)
	{
		if (_newDepthStencilState.depthWriteMask)
		{
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}

		if (_newDepthStencilState.depthEnable)
		{
			uint32 oglDepthFuncs[8] = { GL_LEQUAL, GL_LESS, GL_EQUAL, GL_GREATER, GL_GEQUAL, GL_ALWAYS, GL_ALWAYS, GL_ALWAYS };

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(oglDepthFuncs[_newDepthStencilState.depthFunc]);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		_curDepthStencilState.hash = _newDepthStencilState.hash;
	}

	// Number of vertices in patch. Used in tesselation.
	if (_tessPatchVerts != _lastTessPatchVertsValue)
	{
		glPatchParameteri(GL_PATCH_VERTICES, _tessPatchVerts);

		_lastTessPatchVertsValue = _tessPatchVerts;
	}
}