#include "stdafx.h"

// General
#include "RenderDevice.h"

// Template specializations for vertex buffers.
template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<float> >(const std::vector<float>& data)
{
	return CreateFloatVertexBuffer(&(data[0]), (uint32)data.size(), sizeof(float));
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<vec2> >(const std::vector<vec2>& data)
{
	return CreateFloatVertexBuffer(glm::value_ptr(data[0]), (uint32)data.size(), sizeof(vec2));
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<vec3> >(const std::vector<vec3>& data)
{
	return CreateFloatVertexBuffer(glm::value_ptr(data[0]), (uint32)data.size(), sizeof(vec3));
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<vec4> >(const std::vector<vec4>& data)
{
	return CreateFloatVertexBuffer(glm::value_ptr(data[0]), (uint32)data.size(), sizeof(vec4));
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateIndexBuffer< std::vector<uint16> >(const std::vector<uint16>& data)
{
	return CreateUInt16IndexBuffer(&(data[0]), (uint16)data.size());
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateIndexBuffer< std::vector<uint32> >(const std::vector<uint32>& data)
{
	return CreateUInt32IndexBuffer(&(data[0]), (uint32)data.size());
}