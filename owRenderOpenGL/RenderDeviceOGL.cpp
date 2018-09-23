#include <stdafx.h>

// Include
#include "BufferOGL.h"
#include "ConstantBufferOGL.h"
#include "StructuredBufferOGL.h"
#include "RenderTargetOGL.h"
#include "MeshOGL.h"
#include "ShaderOGL.h"
#include "TextureOGL.h"
#include "SamplerStateOGL.h"
#include "PipelineStateOGL.h"
#include "QueryOGL.h"

// General
#include "RenderDeviceOGL.h"

#ifdef _DEBUG
void _stdcall glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	//if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	//  Dynamic buff	pixel sync
	if (id == 131185 || id == 131154) return;


	Log::Error("-----------------------------------------------------");
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

	Log::Error("");

	_ASSERT(false);
}
#endif

RenderDeviceOGL::RenderDeviceOGL() :
	m_DeviceCapsSettings(GetSettingsGroup<CGroupRenderCaps>()),
	m_OpenGLSettings(GetSettingsGroup<CGroupOpenGL>())
{
	//CreateDevice();

	//LoadDefaultResources();
}

RenderDeviceOGL::~RenderDeviceOGL()
{
	//TwTerminate();

	m_Materials.clear();
	m_Scenes.clear();
	m_Meshes.clear();
	m_Buffers.clear();
	m_Shaders.clear();
	m_Textures.clear();
	m_Samplers.clear();
	m_Pipelines.clear();
	m_Queries.clear();
}

void RenderDeviceOGL::CreateDevice(HDC _hdc)
{
	bool failed = false;

	HGLRC glrc = initOpenGL(_hdc);

	Log::Info("Initializing GL4 backend.");
	if (!initOGLExtensions())
	{
		Log::Error("Could not find all required OpenGL function entry points");
		failed = true;
	}

	char* vendor = (char*)glGetString(GL_VENDOR);
	char* renderer = (char*)glGetString(GL_RENDERER);
	char* version = (char*)glGetString(GL_VERSION);

	if (!version || !renderer || !vendor)
	{
		Log::Error("OpenGL not initialized. Make sure you have a valid OpenGL context");
		return;
	}

	Log::Info("OpenGL driver[%s] by[%s] on[%s]", version, vendor, renderer);

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
		//return false;
	}

	// Debug output
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
#endif

	// Set capabilities
	m_DeviceCapsSettings.texFloat = true;
	m_DeviceCapsSettings.texNPOT = true;
	m_DeviceCapsSettings.rtMultisampling = true;
	m_DeviceCapsSettings.geometryShaders = true;
	m_DeviceCapsSettings.tesselation = m_OpenGLSettings.majorVersion >= 4 && m_OpenGLSettings.minorVersion >= 1;
	m_DeviceCapsSettings.computeShaders = m_OpenGLSettings.majorVersion >= 4 && m_OpenGLSettings.minorVersion >= 3;

	// Find maximum number of storage buffers in compute shader
	glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, (GLint *)&_maxComputeBufferAttachments);

	// Init states before creating test render buffer, to ensure binding the current FBO again
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		GLint value;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &value);
		m_IsDefaultFBOMultisampled = value > 0;

		GLboolean doubleBuffered;
		glGetBooleanv(GL_DOUBLEBUFFER, &doubleBuffered);
		_doubleBuffered = doubleBuffered != 0;

		// Get the currently bound frame buffer object to avoid reset to invalid FBO
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_RBDefault);
	}

	// Find supported depth format (some old ATI cards only support 16 bit depth for FBOs)
	{
		m_DepthFormat = GL_DEPTH_COMPONENT24;
		/*Log::Info("Render target depth precision limited to 32 bit");
		std::shared_ptr<R_RenderBuffer> testBuf32 = createRenderBuffer(32, 32, R_TextureFormats::RGBA8, true, 1, 0);
		if (testBuf32 == nullptr)
		{
			m_DepthFormat = GL_DEPTH_COMPONENT24;
			Log::Warn("Render target depth precision limited to 24 bit");
			std::shared_ptr<R_RenderBuffer> testBuf24 = createRenderBuffer(32, 32, R_TextureFormats::RGBA8, true, 1, 0);
			if (testBuf24 == nullptr)
			{
				m_DepthFormat = GL_DEPTH_COMPONENT16;
				Log::Error("Render target depth precision limited to 16 bit");
			}
		}*/
	}

	m_DeviceName = std::string(vendor) + std::string(renderer) + std::string(version);
}

cstring RenderDeviceOGL::GetDeviceName() const
{
	return m_DeviceName;
}

std::shared_ptr<Buffer> RenderDeviceOGL::CreateFloatVertexBuffer(const float* data, uint32 count, uint32 stride)
{
	std::shared_ptr<Buffer> buffer = std::make_shared<BufferOGL>(GL_ARRAY_BUFFER, data, count, stride);
	m_Buffers.push_back(buffer);

	return buffer;
}

std::shared_ptr <Buffer> RenderDeviceOGL::CreateDoubleVertexBuffer(const double* data, uint32 count, uint32 stride)
{
	std::shared_ptr<Buffer> buffer = std::make_shared<BufferOGL>(GL_ARRAY_BUFFER, data, count, stride);
	m_Buffers.push_back(buffer);

	return buffer;
}

std::shared_ptr<Buffer> RenderDeviceOGL::CreateUInt16IndexBuffer(const uint16* data, uint32 count)
{
	std::shared_ptr <Buffer> buffer = std::make_shared<BufferOGL>(GL_ELEMENT_ARRAY_BUFFER, data, count, (UINT)sizeof(uint16));
	m_Buffers.push_back(buffer);

	return buffer;
}

std::shared_ptr<Buffer> RenderDeviceOGL::CreateUInt32IndexBuffer(const uint32* data, uint32 count)
{
	std::shared_ptr <Buffer> buffer = std::make_shared<BufferOGL>(GL_ELEMENT_ARRAY_BUFFER, data, count, (UINT)sizeof(uint32));
	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceOGL::DestroyBuffer(std::shared_ptr<Buffer> buffer)
{
	BufferList::iterator iter = std::find(m_Buffers.begin(), m_Buffers.end(), buffer);
	if (iter != m_Buffers.end())
	{
		m_Buffers.erase(iter);
	}
}

void RenderDeviceOGL::DestroyVertexBuffer(std::shared_ptr<Buffer> buffer)
{
	DestroyBuffer(buffer);
}

void RenderDeviceOGL::DestroyIndexBuffer(std::shared_ptr<Buffer> buffer)
{
	DestroyBuffer(buffer);
}

//--

std::shared_ptr<ConstantBuffer> RenderDeviceOGL::CreateConstantBuffer(const void* data, size_t size)
{
	std::shared_ptr<ConstantBuffer> buffer = std::make_shared<ConstantBufferOGL>(size);

	if (data)
	{
		buffer->Set(data, size);
	}

	m_Buffers.push_back(buffer);

	return buffer;
}
void RenderDeviceOGL::DestroyConstantBuffer(std::shared_ptr<ConstantBuffer> buffer)
{
	DestroyBuffer(buffer);
}

//--

std::shared_ptr<StructuredBuffer> RenderDeviceOGL::CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<StructuredBuffer> buffer = std::make_shared<StructuredBufferOGL>(GL_SHADER_STORAGE_BUFFER, data, count, stride, cpuAccess, gpuWrite);
	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceOGL::DestroyStructuredBuffer(std::shared_ptr<StructuredBuffer> buffer)
{
	DestroyBuffer(buffer);
}

//--

// GLM's own quaternion from two vector constructor does not handle cases 
// where the vectors may be pointing in opposite directions.
// This method handles the cases where the u and v vectors are opposites.
// source: http://lolengine.net/blog/2014/02/24/quaternion-from-two-vectors-final
// accessed: 26/05/2015
inline glm::quat RotationFromTwoVectors(cvec3 u, cvec3 v)
{
	float normUV = glm::sqrt(glm::dot(u, u) * glm::dot(v, v));
	float real = normUV + glm::dot(u, v);

	vec3 vec;

	if (real < 1.e-6f * normUV)
	{
		/* If u and v are exactly opposite, rotate 180 degrees
		* around an arbitrary orthogonal axis. Axis normalisation
		* can happen later, when we normalise the quaternion.
		*/
		real = 0.0f;
		vec = (glm::abs(u.x) > abs(u.z)) ? vec3(-u.y, u.x, 0.0f) : vec3(0.0f, -u.z, u.y);
	}
	else
	{
		/* Otherwise, build quaternion the standard way. */
		vec = glm::cross(u, v);
	}

	return glm::normalize(glm::quat(real, vec));
}

std::shared_ptr<Mesh> RenderDeviceOGL::CreatePlane(float size, cvec3 N)
{
	vec3 p[4];
	p[0] = vec3(1.0f, 0, 1.0f);
	p[1] = vec3(-1.0f, 0, 1.0f);
	p[2] = vec3(-1.0f, 0, -1.0f);
	p[3] = vec3(1.0f, 0, -1.0f);

	vec2 t[4];
	t[0] = vec2(1, 1);
	t[1] = vec2(0, 1);
	t[2] = vec2(0, 0);
	t[3] = vec2(1, 0);

	uint16 i[6];
	i[0] = 0;
	i[1] = 1;
	i[2] = 2;
	i[3] = 2;
	i[4] = 3;
	i[5] = 0;

	std::shared_ptr<Mesh> mesh = CreateMesh();

	std::shared_ptr<Buffer> __vb = CreateFloatVertexBuffer((const float*)p, 4, sizeof(vec3));
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<Buffer> __tb = CreateFloatVertexBuffer((const float*)t, 4, sizeof(vec2));
	mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __tb);

	std::shared_ptr<Buffer> __ib = CreateUInt16IndexBuffer(i, 6);
	mesh->SetIndexBuffer(__ib);

	std::shared_ptr<Material> mat = CreateMaterial();
	mat->SetDiffuseColor(vec4(1, 0, 0, 1));
	mesh->SetMaterial(mat);

	return mesh;
}

std::shared_ptr<Mesh> RenderDeviceOGL::CreateScreenQuad(float left, float right, float bottom, float top, float z)
{
	vec3 p[4]; // Vertex position
	vec3 n[4]; // Vertex normal (required for texture patch polygons)
	vec2 t[4]; // Texture coordinates
			   // Winding order is assumed to be right-handed. Front-facing polygons have
			   // a counter-clockwise winding order.
			   // Assimp flips the winding order of vertices.. Don't ask me why. To account for this,
			   // the vertices are loaded in reverse order :)
	p[0] = vec3(right, bottom, z);   n[0] = vec3(0, 0, 1);    t[0] = vec2(1, 0);
	p[1] = vec3(left, bottom, z);    n[1] = vec3(0, 0, 1);    t[1] = vec2(0, 0);
	p[2] = vec3(left, top, z);       n[2] = vec3(0, 0, 1);    t[2] = vec2(0, 1);
	p[3] = vec3(right, top, z);      n[3] = vec3(0, 0, 1);    t[3] = vec2(1, 1);

	// Now create the quad.
	/*std::shared_ptr<Scene> scene = CreateScene();

	std::stringstream ss;

	// Create a white diffuse material for the quad.
	// f red green blue Kd Ks Shine transmittance indexOfRefraction
	ss << "f 1 1 1 1 0 0 0 0" << std::endl;

	// Create a 4-point textured polygon patch
	ss << "tpp 4" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		// px py pz nx ny nz tu tv
		ss << p[i].x << " " << p[i].y << " " << p[i].z << " " << n[i].x << " " << n[i].y << " " << n[i].z << " " << t[i].x << " " << t[i].y << std::endl;
	}

	if (scene->LoadFromString(ss.str(), "nff"))
	{
	return scene;
	}

	// An error occurred while loading the scene.
	DestroyScene(scene);*/
	return nullptr;

}

std::shared_ptr<Mesh> RenderDeviceOGL::CreateSphere(float radius, float tesselation)
{
	vec3 spVerts[126] =
	{  // x, y, z
		vec3(0.0f, 1.0f, 0.0f),        vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.707f, 0.0f, 0.707f),   vec3(0.707f, 0.0f, 0.707f),
		vec3(0.707f, 0.0f, -0.707f),   vec3(-0.707f, 0.0f, -0.707f)
	};

	uint16 spInds[128 * 3] = {  // Number of faces: (4 ^ iterations) * 8
		2, 3, 0,   3, 4, 0,   4, 5, 0,   5, 2, 0,   2, 1, 3,   3, 1, 4,   4, 1, 5,   5, 1, 2
	};

	for (uint32 i = 0, nv = 6, ni = 24; i < 2; ++i)  // Two iterations
	{
		// Subdivide each face into 4 tris by bisecting each edge and push vertices onto unit sphere
		for (uint32 j = 0, prevNumInds = ni; j < prevNumInds; j += 3)
		{
			spVerts[nv++] = glm::normalize(((spVerts[spInds[j + 0]] + spVerts[spInds[j + 1]]) * 0.5f));
			spVerts[nv++] = glm::normalize(((spVerts[spInds[j + 1]] + spVerts[spInds[j + 2]]) * 0.5f));
			spVerts[nv++] = glm::normalize(((spVerts[spInds[j + 2]] + spVerts[spInds[j + 0]]) * 0.5f));

			spInds[ni++] = spInds[j + 0];
			spInds[ni++] = nv - 3;
			spInds[ni++] = nv - 1;

			spInds[ni++] = nv - 3;
			spInds[ni++] = spInds[j + 1];
			spInds[ni++] = nv - 2;

			spInds[ni++] = nv - 2;
			spInds[ni++] = spInds[j + 2];
			spInds[ni++] = nv - 1;

			spInds[j + 0] = nv - 3;
			spInds[j + 1] = nv - 2;
			spInds[j + 2] = nv - 1;
		}
	}

	std::shared_ptr<Mesh> mesh = CreateMesh();

	std::shared_ptr<Buffer> __vb = CreateFloatVertexBuffer((const float*)spVerts, 126, sizeof(vec3));
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<Buffer> __ib = CreateUInt16IndexBuffer((const uint16*)spInds, 128 * 3);
	mesh->SetIndexBuffer(__ib);

	std::shared_ptr<Material> mat = CreateMaterial();
	mat->SetDiffuseColor(vec4(1, 0, 0, 1));
	mesh->SetMaterial(mat);

	return mesh;
}

std::shared_ptr<Mesh> RenderDeviceOGL::CreateCube(float size)
{
	float cubeVerts[8 * 3] = {  // x, y, z
		0.f, 0.f, 1.f,   1.f, 0.f, 1.f,   1.f, 1.f, 1.f,   0.f, 1.f, 1.f,
		0.f, 0.f, 0.f,   1.f, 0.f, 0.f,   1.f, 1.f, 0.f,   0.f, 1.f, 0.f
	};
	uint16 cubeInds[36] = {
		0, 1, 2, 2, 3, 0,   1, 5, 6, 6, 2, 1,   5, 4, 7, 7, 6, 5,
		4, 0, 3, 3, 7, 4,   3, 2, 6, 6, 7, 3,   4, 5, 1, 1, 0, 4
	};

	std::shared_ptr<Mesh> mesh = CreateMesh();

	std::shared_ptr<Buffer> __vb = CreateFloatVertexBuffer((const float*)cubeVerts, 8 * 3, sizeof(vec3));
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<Buffer> __ib = CreateUInt16IndexBuffer((const uint16*)cubeInds, 36);
	mesh->SetIndexBuffer(__ib);

	std::shared_ptr<Material> mat = CreateMaterial();
	mat->SetDiffuseColor(vec4(1, 0, 0, 1));
	mesh->SetMaterial(mat);

	return mesh;
}

std::shared_ptr<Mesh> RenderDeviceOGL::CreateCylinder(float baseRadius, float apexRadius, float height, cvec3 axis)
{
	throw std::exception("Not implemented!");
}

std::shared_ptr<Mesh> RenderDeviceOGL::CreateCone(float baseRadius, float height)
{
	throw std::exception("Not implemented!");
}

std::shared_ptr<Mesh> RenderDeviceOGL::CreateArrow(cvec3 tail, cvec3 head, float radius)
{
	throw std::exception("Not implemented!");
}

std::shared_ptr<Mesh> RenderDeviceOGL::CreateAxis(float radius, float length)
{
	throw std::exception("Not implemented!");
}

std::shared_ptr<Mesh> RenderDeviceOGL::CreateMesh()
{
	std::shared_ptr<Mesh> mesh = std::make_shared<MeshOGL>();
	m_Meshes.push_back(mesh);

	return mesh;
}

void RenderDeviceOGL::DestroyMesh(std::shared_ptr<Mesh> mesh)
{
	MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	if (iter != m_Meshes.end())
	{
		m_Meshes.erase(iter);
	}
}

std::shared_ptr<Shader> RenderDeviceOGL::CreateShader()
{
	std::shared_ptr<Shader> pShader = std::make_shared<ShaderOGL>();
	m_Shaders.push_back(pShader);

	return pShader;
}

void RenderDeviceOGL::DestroyShader(std::shared_ptr<Shader> shader)
{
	ShaderList::iterator iter = std::find(m_Shaders.begin(), m_Shaders.end(), shader);
	if (iter != m_Shaders.end())
	{
		m_Shaders.erase(iter);
	}
}


std::shared_ptr<Texture> RenderDeviceOGL::CreateTexture(cstring fileName)
{
	TextureMap::iterator iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		return iter->second;
	}

	std::shared_ptr<Texture> texture = std::make_shared<TextureOGL>(this);
	texture->LoadTexture2D(fileName);

	m_Textures.push_back(texture);
	m_TexturesByName.insert(TextureMap::value_type(fileName, texture));

	return texture;
}

std::shared_ptr<Texture> RenderDeviceOGL::CreateTextureCube(cstring fileName)
{
	TextureMap::iterator iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		return iter->second;
	}

	std::shared_ptr<Texture> texture = std::make_shared<TextureOGL>(this);
	texture->LoadTextureCube(fileName);

	m_Textures.push_back(texture);
	m_TexturesByName.insert(TextureMap::value_type(fileName, texture));

	return texture;

}

std::shared_ptr<Texture> RenderDeviceOGL::CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices, const Texture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<Texture> texture = std::make_shared<TextureOGL>(this, width, height, slices, format, cpuAccess, gpuWrite);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<Texture> RenderDeviceOGL::CreateTextureCube(uint16_t size, uint16_t numCubes, const Texture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<Texture> texture = std::make_shared<TextureOGL>(this, size, numCubes, format, cpuAccess, gpuWrite);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<Texture> RenderDeviceOGL::CreateTexture()
{
	std::shared_ptr<Texture> texture = std::make_shared<TextureOGL>(this);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<Texture> RenderDeviceOGL::GetDefaultTexture() const
{
	return m_pDefaultTexture;
}

void RenderDeviceOGL::DestroyTexture(std::shared_ptr<Texture> texture)
{
	TextureList::iterator iter = std::find(m_Textures.begin(), m_Textures.end(), texture);
	if (iter != m_Textures.end())
	{
		m_Textures.erase(iter);
	}

	TextureMap::iterator iter2 = std::find_if(m_TexturesByName.begin(), m_TexturesByName.end(), [=](TextureMap::value_type val) { return (val.second == texture); });
	if (iter2 != m_TexturesByName.end())
	{
		m_TexturesByName.erase(iter2);
	}
}


std::shared_ptr<RenderTarget> RenderDeviceOGL::CreateRenderTarget()
{
	std::shared_ptr<RenderTargetOGL> renderTarget = std::make_shared<RenderTargetOGL>(this);
	m_RenderTargets.push_back(renderTarget);

	return renderTarget;
}

void RenderDeviceOGL::DestroyRenderTarget(std::shared_ptr<RenderTarget> renderTarget)
{
	RenderTargetList::iterator iter = std::find(m_RenderTargets.begin(), m_RenderTargets.end(), renderTarget);
	if (iter != m_RenderTargets.end())
	{
		m_RenderTargets.erase(iter);
	}
}


std::shared_ptr<SamplerState> RenderDeviceOGL::CreateSamplerState()
{
	std::shared_ptr<SamplerState> sampler = std::make_shared<SamplerStateOGL>();
	m_Samplers.push_back(sampler);

	return sampler;
}

void RenderDeviceOGL::DestroySampler(std::shared_ptr<SamplerState> sampler)
{
	SamplerList::iterator iter = std::find(m_Samplers.begin(), m_Samplers.end(), sampler);
	if (iter != m_Samplers.end())
	{
		m_Samplers.erase(iter);
	}
}


std::shared_ptr<Material> RenderDeviceOGL::CreateMaterial()
{
	std::shared_ptr<Material> pMaterial = std::make_shared<Material>(*this);
	m_Materials.push_back(pMaterial);
	return pMaterial;
}

void RenderDeviceOGL::DestroyMaterial(std::shared_ptr<Material> material)
{
	MaterialList::iterator iter = std::find(m_Materials.begin(), m_Materials.end(), material);
	if (iter != m_Materials.end())
	{
		m_Materials.erase(iter);
	}
}


std::shared_ptr<PipelineState> RenderDeviceOGL::CreatePipelineState()
{
	std::shared_ptr<PipelineState> pPipeline = std::make_shared<PipelineStateOGL>();
	m_Pipelines.push_back(pPipeline);

	return pPipeline;
}

void RenderDeviceOGL::DestoryPipelineState(std::shared_ptr<PipelineState> pipeline)
{
	PipelineList::iterator iter = std::find(m_Pipelines.begin(), m_Pipelines.end(), pipeline);
	if (iter != m_Pipelines.end())
	{
		m_Pipelines.erase(iter);
	}
}

std::shared_ptr<PipelineState> RenderDeviceOGL::GetDefaultPipeline() const
{
	return m_pDefaultPipeline;
}

bool RenderDeviceOGL::IsDoubleBuffered()
{
	return _doubleBuffered;
}

uint32 RenderDeviceOGL::GetDefaultRB()
{
	return m_RBDefault;
}

void RenderDeviceOGL::SetDefaultRB(uint32 _obj)
{
	m_RBDefault = _obj;
}



std::shared_ptr<Query> RenderDeviceOGL::CreateQuery(Query::QueryType queryType, uint8_t numBuffers)
{
	std::shared_ptr<Query> query = std::make_shared<QueryOGL>(queryType, numBuffers);
	m_Queries.push_back(query);

	return query;
}

void RenderDeviceOGL::DestoryQuery(std::shared_ptr<Query> query)
{
	QueryList::iterator iter = std::find(m_Queries.begin(), m_Queries.end(), query);
	if (iter != m_Queries.end())
	{
		m_Queries.erase(iter);
	}
}

void RenderDeviceOGL::LoadDefaultResources()
{
	// Load a default shader
	//std::shared_ptr<Shader> pDefaultVertexShader = CreateShader();
	//pDefaultVertexShader->LoadShaderFromFile(Shader::VertexShader, "shaders\\M2\\M2.vs", Shader::ShaderMacros(), "VS_main", "vs_4_0");

	//std::shared_ptr<Shader> pDefaultPixelShader = CreateShader();
	//pDefaultPixelShader->LoadShaderFromFile(Shader::PixelShader, "shaders\\M2\\M2.fs", Shader::ShaderMacros(), "PS_main", "ps_4_0");

	// Create a magenta texture if a texture defined in the shader is not bound.
	m_pDefaultTexture = CreateTexture2D(1, 1, 1, Texture::TextureFormat());
	m_pDefaultTexture->Clear(ClearFlags::Color, vec4(1, 0, 1, 1));

	//m_pDefaultPipeline = CreatePipelineState();
	//m_pDefaultPipeline->SetShader(Shader::VertexShader, pDefaultVertexShader);
	//m_pDefaultPipeline->SetShader(Shader::PixelShader, pDefaultPixelShader);
	// TODO: Default pipeline state must be assigned to a renderwindow
	// because the RenderWindow has a default render target that must be bound to the pipeline.
}