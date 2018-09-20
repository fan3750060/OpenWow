#include <stdafx.h>

#include "BufferDX11.h"
#include "ConstantBufferDX11.h"
#include "StructuredBufferDX11.h"
#include "RenderTargetDX11.h"
#include "MeshDX11.h"
#include "ShaderDX11.h"
#include "TextureDX11.h"
#include "SamplerStateDX11.h"
#include "PipelineStateDX11.h"
#include "QueryDX11.h"

// General
#include "RenderDeviceDX11.h"

RenderDeviceDX11::RenderDeviceDX11()
{
	CreateDevice();

	LoadDefaultResources();

	// Initialize AntTweak bar.
   // TwInit( TW_DIRECT3D11, m_pDevice );
}

RenderDeviceDX11::~RenderDeviceDX11()
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

#if defined(_DEBUG)
	if (m_pDebugLayer)
	{
		//        m_pDebugLayer->ReportLiveDeviceObjects( D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL );
	}
#endif
}

void RenderDeviceDX11::CreateDevice()
{
	const D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
	};

	UINT createDeviceFlags = 0;
#if defined(_DEBUG)
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// This will be the feature level that 
	// is used to create our device and swap chain.
	D3D_FEATURE_LEVEL featureLevel;

	ATL::CComPtr<ID3D11Device> pDevice;
	ATL::CComPtr<ID3D11DeviceContext> pDeviceContext;

	// First create a ID3D11Device and ID3D11DeviceContext
	HRESULT hr = D3D11CreateDevice
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&pDevice,
		&featureLevel,
		&pDeviceContext
	);

	if (hr == E_INVALIDARG)
	{
		hr = D3D11CreateDevice
		(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags,
			&featureLevels[1],
			_countof(featureLevels) - 1,
			D3D11_SDK_VERSION,
			&pDevice,
			&featureLevel,
			&pDeviceContext
		);
	}

	if (FAILED(hr))
	{
		Log::Error("Failed to created DirectX 11 Device");
		return;
	}

	// Now query for the ID3D11Device2 interface.
	if (FAILED(pDevice->QueryInterface<ID3D11Device2>(&m_pDevice)))
	{
		Log::Error("Failed to create DirectX 11.2 device");
	}

	// Now get the immediate device context.
	m_pDevice->GetImmediateContext2(&m_pDeviceContext);

	if (SUCCEEDED(m_pDevice->QueryInterface<ID3D11Debug>(&m_pDebugLayer)))
	{
		ATL::CComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(m_pDebugLayer->QueryInterface<ID3D11InfoQueue>(&d3dInfoQueue)))
		{
#if defined(_DEBUG)
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
			//d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, TRUE);
#endif 
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// Add more message IDs here as needed
			};

			D3D11_INFO_QUEUE_FILTER filter;
			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}

	// Query the adapter information.
	ATL::CComPtr<IDXGIFactory> factory;
	ATL::CComPtr<IDXGIAdapter> adapter;
	DXGI_ADAPTER_DESC adapterDescription = {};

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
	{
		Log::Error("Failed to create DXGIFactory.");
	}
	if (FAILED(factory->EnumAdapters(0, &adapter)))
	{
		Log::Error("Failed to enumerate adapter.");
	}
	if (FAILED(adapter->GetDesc(&adapterDescription)))
	{
		Log::Error("Failed to qauery adapter description.");
	}

	m_DeviceName = ConvertString(adapterDescription.Description);
}

cstring RenderDeviceDX11::GetDeviceName() const
{
	return m_DeviceName;
}

ATL::CComPtr<ID3D11Device2> RenderDeviceDX11::GetDevice() const
{
	return m_pDevice;
}

ATL::CComPtr<ID3D11DeviceContext2> RenderDeviceDX11::GetDeviceContext() const
{
	return m_pDeviceContext;
}


std::shared_ptr<Buffer> RenderDeviceDX11::CreateFloatVertexBuffer(const float* data, uint32 count, uint32 stride)
{
	std::shared_ptr<Buffer> buffer = std::make_shared<BufferDX11>(m_pDevice, D3D11_BIND_VERTEX_BUFFER, data, count, stride);
	m_Buffers.push_back(buffer);

	return buffer;
}

std::shared_ptr <Buffer> RenderDeviceDX11::CreateDoubleVertexBuffer(const double* data, uint32 count, uint32 stride)
{
	std::shared_ptr<Buffer> buffer = std::make_shared<BufferDX11>(m_pDevice, D3D11_BIND_VERTEX_BUFFER, data, count, stride);
	m_Buffers.push_back(buffer);

	return buffer;
}

std::shared_ptr<Buffer> RenderDeviceDX11::CreateUInt16IndexBuffer(const uint16* data, uint32 count)
{
	std::shared_ptr <Buffer> buffer = std::make_shared<BufferDX11>(m_pDevice, D3D11_BIND_INDEX_BUFFER, data, count, (UINT)sizeof(uint16));
	m_Buffers.push_back(buffer);

	return buffer;
}

std::shared_ptr<Buffer> RenderDeviceDX11::CreateUInt32IndexBuffer(const uint32* data, uint32 count)
{
	std::shared_ptr <Buffer> buffer = std::make_shared<BufferDX11>(m_pDevice, D3D11_BIND_INDEX_BUFFER, data, count, (UINT)sizeof(uint32));
	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceDX11::DestroyBuffer(std::shared_ptr<Buffer> buffer)
{
	BufferList::iterator iter = std::find(m_Buffers.begin(), m_Buffers.end(), buffer);
	if (iter != m_Buffers.end())
	{
		m_Buffers.erase(iter);
	}
}

void RenderDeviceDX11::DestroyVertexBuffer(std::shared_ptr<Buffer> buffer)
{
	DestroyBuffer(buffer);
}

void RenderDeviceDX11::DestroyIndexBuffer(std::shared_ptr<Buffer> buffer)
{
	DestroyBuffer(buffer);
}

//--

std::shared_ptr<ConstantBuffer> RenderDeviceDX11::CreateConstantBuffer(const void* data, size_t size)
{
	std::shared_ptr<ConstantBuffer> buffer = std::make_shared<ConstantBufferDX11>(m_pDevice, size);

	if (data)
	{
		buffer->Set(data, size);
	}

	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceDX11::DestroyConstantBuffer(std::shared_ptr<ConstantBuffer> buffer)
{
	DestroyBuffer(buffer);
}

//--

std::shared_ptr<StructuredBuffer> RenderDeviceDX11::CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<StructuredBuffer> buffer = std::make_shared<StructuredBufferDX11>(m_pDevice, 0, data, count, stride, cpuAccess, gpuWrite);
	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceDX11::DestroyStructuredBuffer(std::shared_ptr<StructuredBuffer> buffer)
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

std::shared_ptr<Mesh> RenderDeviceDX11::CreatePlane(float size, cvec3 N)
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

std::shared_ptr<Mesh> RenderDeviceDX11::CreateScreenQuad(float left, float right, float bottom, float top, float z)
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

std::shared_ptr<Mesh> RenderDeviceDX11::CreateSphere(float radius, float tesselation)
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

std::shared_ptr<Mesh> RenderDeviceDX11::CreateCube(float size)
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

std::shared_ptr<Mesh> RenderDeviceDX11::CreateCylinder(float baseRadius, float apexRadius, float height, cvec3 axis)
{
	/*std::shared_ptr<Scene> scene = CreateScene();
	std::stringstream ss;

	// Create a white diffuse material for the cylinder.
	// f red green blue Kd Ks Shine transmittance indexOfRefraction
	ss << "f 1 1 1 1 0 0 0 0" << std::endl;

	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << "0 0 0 " << baseRadius << std::endl;

	vec3 apex = axis * height;
	// apex.x apex.y apex.z apexRadius
	ss << apex.x << " " << apex.y << " " << apex.z << " " << apexRadius << std::endl;

	if (scene->LoadFromString(ss.str(), "nff"))
	{
	return scene;
	}

	// An error occurred while loading the scene.
	DestroyScene(scene);*/
	return nullptr;
}

std::shared_ptr<Mesh> RenderDeviceDX11::CreateCone(float baseRadius, float height)
{
	// A cone is just a cylinder with a 0 size apex.
	return CreateCylinder(baseRadius, 0, height);
}

std::shared_ptr<Mesh> RenderDeviceDX11::CreateArrow(cvec3 tail, cvec3 head, float radius)
{
	/*std::shared_ptr<Scene> scene = CreateScene();
	std::stringstream ss;

	vec3 dir = head - tail;
	vec3 apex = head + (dir * 0.5f);

	// Create a white diffuse material for the arrow.
	// f red green blue Kd Ks Shine transmittance indexOfRefraction
	ss << "f 1 1 1 1 0 0 0 0" << std::endl;

	// Create a cylinder for the arrow body.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << tail.x << " " << tail.y << " " << tail.z << " " << radius << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << head.x << " " << head.y << " " << head.z << " " << radius << std::endl;

	// Create a cone for the arrow head.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << head.x << " " << head.y << " " << head.z << " " << radius * 2.0f << std::endl;

	// apex.x apex.y apex.z apexRadius
	ss << apex.x << " " << apex.y << " " << apex.z << " 0" << std::endl;

	if (scene->LoadFromString(ss.str(), "nff"))
	{
	return scene;
	}*/

	// An error occurred while loading the scene.
	return nullptr;

}

std::shared_ptr<Mesh> RenderDeviceDX11::CreateAxis(float radius, float length)
{
	/*std::shared_ptr<Scene> scene = CreateScene();
	std::stringstream ss;

	// Create a red material for the +X axis.
	// f red green blue Kd Ks Shine transmittance indexOfRefraction
	ss << "f 1 0 0 1 0 0 0 0" << std::endl;

	// Create a cylinder aligned to the +X axis.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << "0 0 0 " << radius << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << length << " 0 0 " << radius << std::endl;

	// Create a cone for the +X axis.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << length << " 0 0 " << radius * 2.0f << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << length * 1.5f << " 0 0 0" << std::endl;

	// Create a green material for the +Y axis.
	// f red green blue Kd Ks Shine transmittance indexOfRefraction
	ss << "f 0 1 0 1 0 0 0 0" << std::endl;

	// Create a cylinder aligned to the +Y axis.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << "0 0 0 " << radius << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << "0 " << length << " 0 " << radius << std::endl;

	// Create a cone for the +Y axis.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << "0 " << length << " 0 " << radius * 2.0f << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << "0 " << length * 1.5f << " 0 0" << std::endl;

	// Create a blue material for the +Z axis.
	// f red green blue Kd Ks Shine transmittance indexOfRefraction
	ss << "f 0 0 1 1 0 0 0 0" << std::endl;

	// Create a cylinder aligned to the +Z axis.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << "0 0 0 " << radius << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << "0 0 " << length << " " << radius << std::endl;

	// Create a cone for the +Z axis.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << "0 0 " << length << " " << radius * 2.0f << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << "0 0 " << length * 1.5f << " 0" << std::endl;

	// Create a cyan material for the -X axis.
	// f red green blue Kd Ks Shine transmittance indexOfRefraction
	ss << "f 0 1 1 1 0 0 0 0" << std::endl;

	// Create a cylinder aligned to the -X axis.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << "0 0 0 " << radius << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << -length << " 0 0 " << radius << std::endl;

	// Create a cone for the -X axis.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << -length << " 0 0 " << radius * 2.0f << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << -length * 1.5f << " 0 0 0" << std::endl;

	// Create a yellow material for the -Y axis.
	// f red green blue Kd Ks Shine transmittance indexOfRefraction
	ss << "f 1 0 1 1 0 0 0 0" << std::endl;

	// Create a cylinder aligned to the -Y axis.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << "0 0 0 " << radius << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << "0 " << -length << " 0 " << radius << std::endl;

	// Create a cone for the -Y axis.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << "0 " << -length << " 0 0" << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << "0 " << -length * 1.5f << " 0 " << radius * 2.0f << std::endl;

	// Create a magenta material for the -Z axis.
	// f red green blue Kd Ks Shine transmittance indexOfRefraction
	ss << "f 1 1 0 1 0 0 0 0" << std::endl;

	// Create a cylinder aligned to the -Z axis.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << "0 0 0 " << radius << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << "0 0 " << -length << " " << radius << std::endl;

	// Create a cone for the -Z axis.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << "0 0 " << -length << " " << radius * 2.0f << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << "0 0 " << -length * 1.5f << " 0" << std::endl;

	if (scene->LoadFromString(ss.str(), "nff"))
	{
	return scene;
	}

	// An error occurred while loading the scene.
	*/
	return nullptr;
}

std::shared_ptr<Mesh> RenderDeviceDX11::CreateMesh()
{
	std::shared_ptr<Mesh> mesh = std::make_shared<MeshDX11>(m_pDevice);
	m_Meshes.push_back(mesh);

	return mesh;
}

void RenderDeviceDX11::DestroyMesh(std::shared_ptr<Mesh> mesh)
{
	MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	if (iter != m_Meshes.end())
	{
		m_Meshes.erase(iter);
	}
}

std::shared_ptr<Shader> RenderDeviceDX11::CreateShader()
{
	std::shared_ptr<Shader> pShader = std::make_shared<ShaderDX11>(m_pDevice);
	m_Shaders.push_back(pShader);

	return pShader;
}

void RenderDeviceDX11::DestroyShader(std::shared_ptr<Shader> shader)
{
	ShaderList::iterator iter = std::find(m_Shaders.begin(), m_Shaders.end(), shader);
	if (iter != m_Shaders.end())
	{
		m_Shaders.erase(iter);
	}
}


std::shared_ptr<Texture> RenderDeviceDX11::CreateTexture(cstring fileName)
{
	TextureMap::iterator iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		return iter->second;
	}

	std::shared_ptr<Texture> texture = std::make_shared<TextureDX11>(m_pDevice);
	texture->LoadTexture2D(fileName);

	m_Textures.push_back(texture);
	m_TexturesByName.insert(TextureMap::value_type(fileName, texture));

	return texture;
}

std::shared_ptr<Texture> RenderDeviceDX11::CreateTextureCube(cstring fileName)
{
	TextureMap::iterator iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		return iter->second;
	}

	std::shared_ptr<Texture> texture = std::make_shared<TextureDX11>(m_pDevice);
	texture->LoadTextureCube(fileName);

	m_Textures.push_back(texture);
	m_TexturesByName.insert(TextureMap::value_type(fileName, texture));

	return texture;

}

std::shared_ptr<Texture> RenderDeviceDX11::CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices, const Texture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<Texture> texture = std::make_shared<TextureDX11>(m_pDevice, width, height, slices, format, cpuAccess, gpuWrite);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<Texture> RenderDeviceDX11::CreateTextureCube(uint16_t size, uint16_t numCubes, const Texture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<Texture> texture = std::make_shared<TextureDX11>(m_pDevice, size, numCubes, format, cpuAccess, gpuWrite);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<Texture> RenderDeviceDX11::CreateTexture()
{
	std::shared_ptr<Texture> texture = std::make_shared<TextureDX11>(m_pDevice);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<Texture> RenderDeviceDX11::GetDefaultTexture() const
{
	return m_pDefaultTexture;
}

void RenderDeviceDX11::DestroyTexture(std::shared_ptr<Texture> texture)
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


std::shared_ptr<RenderTarget> RenderDeviceDX11::CreateRenderTarget()
{
	std::shared_ptr<RenderTargetDX11> renderTarget = std::make_shared<RenderTargetDX11>(m_pDevice);
	m_RenderTargets.push_back(renderTarget);

	return renderTarget;
}

void RenderDeviceDX11::DestroyRenderTarget(std::shared_ptr<RenderTarget> renderTarget)
{
	RenderTargetList::iterator iter = std::find(m_RenderTargets.begin(), m_RenderTargets.end(), renderTarget);
	if (iter != m_RenderTargets.end())
	{
		m_RenderTargets.erase(iter);
	}
}


std::shared_ptr<SamplerState> RenderDeviceDX11::CreateSamplerState()
{
	std::shared_ptr<SamplerState> sampler = std::make_shared<SamplerStateDX11>(m_pDevice);
	m_Samplers.push_back(sampler);

	return sampler;
}

void RenderDeviceDX11::DestroySampler(std::shared_ptr<SamplerState> sampler)
{
	SamplerList::iterator iter = std::find(m_Samplers.begin(), m_Samplers.end(), sampler);
	if (iter != m_Samplers.end())
	{
		m_Samplers.erase(iter);
	}
}


std::shared_ptr<Material> RenderDeviceDX11::CreateMaterial()
{
	std::shared_ptr<Material> pMaterial = std::make_shared<Material>(*this);
	m_Materials.push_back(pMaterial);
	return pMaterial;
}

void RenderDeviceDX11::DestroyMaterial(std::shared_ptr<Material> material)
{
	MaterialList::iterator iter = std::find(m_Materials.begin(), m_Materials.end(), material);
	if (iter != m_Materials.end())
	{
		m_Materials.erase(iter);
	}
}


std::shared_ptr<PipelineState> RenderDeviceDX11::CreatePipelineState()
{
	std::shared_ptr<PipelineState> pPipeline = std::make_shared<PipelineStateDX11>(m_pDevice);
	m_Pipelines.push_back(pPipeline);

	return pPipeline;
}

void RenderDeviceDX11::DestoryPipelineState(std::shared_ptr<PipelineState> pipeline)
{
	PipelineList::iterator iter = std::find(m_Pipelines.begin(), m_Pipelines.end(), pipeline);
	if (iter != m_Pipelines.end())
	{
		m_Pipelines.erase(iter);
	}
}

std::shared_ptr<PipelineState> RenderDeviceDX11::GetDefaultPipeline() const
{
	return m_pDefaultPipeline;
}


std::shared_ptr<Query> RenderDeviceDX11::CreateQuery(Query::QueryType queryType, uint8_t numBuffers)
{
	std::shared_ptr<Query> query = std::make_shared<QueryDX11>(m_pDevice, queryType, numBuffers);
	m_Queries.push_back(query);

	return query;
}

void RenderDeviceDX11::DestoryQuery(std::shared_ptr<Query> query)
{
	QueryList::iterator iter = std::find(m_Queries.begin(), m_Queries.end(), query);
	if (iter != m_Queries.end())
	{
		m_Queries.erase(iter);
	}
}

void RenderDeviceDX11::LoadDefaultResources()
{
	// Load a default shader
	std::shared_ptr<Shader> pDefaultVertexShader = CreateShader();
	pDefaultVertexShader->LoadShaderFromFile(Shader::VertexShader, "shaders_D3D\\DefaultShader.hlsl", Shader::ShaderMacros(), "VS_main", "vs_4_0");

	std::shared_ptr<Shader> pDefaultPixelShader = CreateShader();
	pDefaultPixelShader->LoadShaderFromFile(Shader::PixelShader, "shaders_D3D\\DefaultShader.hlsl", Shader::ShaderMacros(), "PS_main", "ps_4_0");

	// Create a magenta texture if a texture defined in the shader is not bound.
	m_pDefaultTexture = CreateTexture2D(1, 1, 1, Texture::TextureFormat());
	m_pDefaultTexture->Clear(ClearFlags::Color, vec4(1, 0, 1, 1));

	m_pDefaultPipeline = CreatePipelineState();
	m_pDefaultPipeline->SetShader(Shader::VertexShader, pDefaultVertexShader);
	m_pDefaultPipeline->SetShader(Shader::PixelShader, pDefaultPixelShader);
	// TODO: Default pipeline state must be assigned to a renderwindow
	// because the RenderWindow has a default render target that must be bound to the pipeline.
}