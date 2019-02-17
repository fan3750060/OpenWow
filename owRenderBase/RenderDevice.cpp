#include "stdafx.h"

// General
#include "RenderDevice.h"

std::shared_ptr<IMesh> IRenderDevice::CreatePlane(cvec3 N)
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

	std::shared_ptr<IMesh> mesh = CreateMesh();

	std::shared_ptr<IBuffer> __vb = CreateVertexBuffer(p, 4);
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<IBuffer> __tb = CreateVertexBuffer(t, 4);
	mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __tb);

	std::shared_ptr<IBuffer> __ib = CreateIndexBuffer(i, 6);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

std::shared_ptr<IMesh> IRenderDevice::CreateScreenQuad(float left, float right, float bottom, float top, float z)
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

	uint16 i[6];
	i[0] = 0;
	i[1] = 1;
	i[2] = 2;
	i[3] = 2;
	i[4] = 3;
	i[5] = 0;

	std::shared_ptr<IMesh> mesh = CreateMesh();

	std::shared_ptr<IBuffer> __vb = CreateVertexBuffer(p, 4);
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<IBuffer> __tb = CreateVertexBuffer(t, 4);
	mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __tb);

	std::shared_ptr<IBuffer> __nb = CreateVertexBuffer(n, 4);
	mesh->AddVertexBuffer(BufferBinding("NORMAL", 0), __nb);

	std::shared_ptr<IBuffer> __ib = CreateIndexBuffer(i, 6);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

std::shared_ptr<IMesh> IRenderDevice::CreateSphere()
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

	std::shared_ptr<IMesh> mesh = CreateMesh();

	std::shared_ptr<IBuffer> __vb = CreateVertexBuffer(spVerts, 126);
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<IBuffer> __ib = CreateIndexBuffer(spInds, 128 * 3);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

std::shared_ptr<IMesh> IRenderDevice::CreateCube()
{
	vec3 cubeVerts[8] = {  // x, y, z
		vec3(-0.5f, 0.5f, -0.5f),
		vec3(0.5f, 0.5f, -0.5f),
		 vec3(-0.5f, -0.5f, -0.5f),
		 vec3(0.5f, -0.5f, -0.5f),
		 vec3(-0.5f, 0.5f, 0.5f),
		 vec3(0.5f, 0.5f, 0.5f),
		 vec3(-0.5f, -0.5f, 0.5f),
		 vec3(0.5f, -0.5f, 0.5f)
	};
	uint16 cubeInds[36] = {
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 0, 6,    // side 2
		6, 0, 2,
		7, 5, 6,    // side 3
		6, 5, 4,
		3, 1, 7,    // side 4
		7, 1, 5,
		4, 5, 0,    // side 5
		0, 5, 1,
		3, 7, 2,    // side 6
		2, 7, 6
	};

	std::shared_ptr<IMesh> mesh = CreateMesh();
	mesh->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	std::shared_ptr<IBuffer> __vb = CreateVertexBuffer(cubeVerts, 8);
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<IBuffer> __ib = CreateIndexBuffer(cubeInds, 36);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

std::shared_ptr<IMesh> IRenderDevice::CreateCylinder(float baseRadius, float apexRadius, float height, cvec3 axis)
{
	/*std::shared_ptr<Scene3D> scene = CreateScene();
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

std::shared_ptr<IMesh> IRenderDevice::CreateCone()
{
	float coneVerts[13 * 3] = {  // x, y, z
		0.f, 0.f, 0.f,
		0.f, 1.f, -1.f,   -0.5f, 0.866f, -1.f,   -0.866f, 0.5f, -1.f,
		-1.f, 0.f, -1.f,   -0.866f, -0.5f, -1.f,   -0.5f, -0.866f, -1.f,
		0.f, -1.f, -1.f,   0.5f, -0.866f, -1.f,   0.866f, -0.5f, -1.f,
		1.f, 0.f, -1.f,   0.866f, 0.5f, -1.f,   0.5f, 0.866f, -1.f,
	};
	uint16 coneInds[22 * 3] = {
		0, 1, 2,   0, 2, 3,   0, 3, 4,   0, 4, 5,   0, 5, 6,   0, 6, 7,
		0, 7, 8,   0, 8, 9,   0, 9, 10,   0, 10, 11,   0, 11, 12,   0, 12, 1,
		10, 6, 2,   10, 8, 6,   10, 9, 8,   8, 7, 6,   6, 4, 2,   6, 5, 4,   4, 3, 2,
		2, 12, 10,   2, 1, 12,   12, 11, 10
	};

	std::shared_ptr<IMesh> mesh = CreateMesh();

	std::shared_ptr<IBuffer> __vb = CreateVertexBuffer(coneVerts, 13 * 3);
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<IBuffer> __ib = CreateIndexBuffer(coneInds, 22 * 3);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

// Template specializations for vertex buffers (with std::vector)
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer< std::vector<float> >(const std::vector<float>& data)
{
	return CreateVoidVertexBuffer(&(data[0]), (uint32)data.size(), 0, sizeof(float));
}
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer< std::vector<vec2> >(const std::vector<vec2>& data)
{
	return CreateVoidVertexBuffer(glm::value_ptr(data[0]), (uint32)data.size(), 0, sizeof(vec2));
}
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer< std::vector<vec3> >(const std::vector<vec3>& data)
{
	return CreateVoidVertexBuffer(glm::value_ptr(data[0]), (uint32)data.size(), 0, sizeof(vec3));
}
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer< std::vector<vec4> >(const std::vector<vec4>& data)
{
	return CreateVoidVertexBuffer(glm::value_ptr(data[0]), (uint32)data.size(), 0, sizeof(vec4));
}

// Template specializations for vertex buffers (with common types)
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer<float>(const float* data, uint32 count)
{
	return CreateVoidVertexBuffer(data, count, 0, sizeof(float));
}
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer<vec2>(const vec2* data, uint32 count)
{
	return CreateVoidVertexBuffer((const float*)data, count, 0, sizeof(vec2));
}
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer<vec3>(const vec3* data, uint32 count)
{
	return CreateVoidVertexBuffer((const float*)data, count, 0, sizeof(vec3));
}
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateVertexBuffer<vec4>(const vec4* data, uint32 count)
{
	return CreateVoidVertexBuffer((const float*)data, count, 0, sizeof(vec4));
}

// Template specializations for index buffers (with std::vector)
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateIndexBuffer< std::vector<uint16> >(const std::vector<uint16>& data)
{
	return CreateUInt16IndexBuffer(&(data[0]), (uint16)data.size());
}

template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateIndexBuffer< std::vector<uint32> >(const std::vector<uint32>& data)
{
	return CreateUInt32IndexBuffer(&(data[0]), (uint32)data.size());
}

// Template specializations for index buffers (with common types)
template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateIndexBuffer<uint16>(const uint16* data, uint32 count)
{
	return CreateUInt16IndexBuffer(data, count);
}

template<>
std::shared_ptr<IBuffer> IRenderDevice::CreateIndexBuffer<uint32>(const uint32* data, uint32 count)
{
	return CreateUInt32IndexBuffer(data, count);
}
