#include "stdafx.h"

// General
#include "RenderDevice.h"

// Additional
#include "Primitives/Geometry.h"

std::shared_ptr<IMesh> IRenderDevice::CreateLine(cvec3 _dest)
{
	vec3 p[2];
	p[0] = -_dest;
	p[1] = _dest;

	std::shared_ptr<IMesh> mesh = CreateMesh();
	mesh->SetPrimitiveTopology(PrimitiveTopology::LineList);

	std::shared_ptr<IBuffer> __vb = CreateVertexBuffer(p, 2);
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	return mesh;
}

std::shared_ptr<IMesh> IRenderDevice::CreatePlane(cvec3 N)
{
	DirectX::VertexCollection vertices;
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3( 1.0f, 0.0f,  1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(-1.0f, 0.0f,  1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(-1.0f, 0.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(1.0f, 0.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));

	DirectX::IndexCollection indices;
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(3);

	std::shared_ptr<IMesh> mesh = CreateMesh();

	std::shared_ptr<IBuffer> __vb = CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	mesh->SetVertexBuffer(__vb);

	std::shared_ptr<IBuffer> __ib = CreateIndexBuffer(indices);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

std::shared_ptr<IMesh> IRenderDevice::CreateScreenQuad(float left, float right, float bottom, float top, float z)
{
	DirectX::VertexCollection vertices;
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(right, top, z), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(left, top, z), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(left, bottom, z), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(right, bottom, z), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));

	DirectX::IndexCollection indices;
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(3);

	std::shared_ptr<IMesh> mesh = CreateMesh();

	std::shared_ptr<IBuffer> __vb = CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	mesh->SetVertexBuffer(__vb);

	std::shared_ptr<IBuffer> __ib = CreateIndexBuffer(indices);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

std::shared_ptr<IMesh> IRenderDevice::CreateSphere()
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeSphere(vertices, indices, 1.0f, 32, false, false);

	std::shared_ptr<IMesh> mesh = CreateMesh();

	std::shared_ptr<IBuffer> __vb = CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	mesh->SetVertexBuffer(__vb);

	std::shared_ptr<IBuffer> __ib = CreateIndexBuffer(indices);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

std::shared_ptr<IMesh> IRenderDevice::CreateCube()
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeBox(vertices, indices, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), false, false);

	std::shared_ptr<IMesh> mesh = CreateMesh();
	mesh->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	std::shared_ptr<IBuffer> __vb = CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	mesh->SetVertexBuffer(__vb);

	std::shared_ptr<IBuffer> __ib = CreateIndexBuffer(indices);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

std::shared_ptr<IMesh> IRenderDevice::CreateCylinder(float baseRadius, float apexRadius, float height, cvec3 axis)
{
	return nullptr;
}

std::shared_ptr<IMesh> IRenderDevice::CreateCone()
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeCone(vertices, indices, 0.77f, 1.0f, 32, false);

	std::shared_ptr<IMesh> mesh = CreateMesh();
	mesh->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	std::shared_ptr<IBuffer> __vb = CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	mesh->SetVertexBuffer(__vb);

	std::shared_ptr<IBuffer> __ib = CreateIndexBuffer(indices);
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
