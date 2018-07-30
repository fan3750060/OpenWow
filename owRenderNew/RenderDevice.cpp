#include "stdafx.h"


#include "RenderDevice.h"

// Template specializations for vertex buffers.
template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<float> >(const std::vector<float>& data)
{
	return CreateFloatVertexBuffer(&(data[0]), (unsigned int)data.size(), sizeof(float));
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<vec2> >(const std::vector<vec2>& data)
{
	return CreateFloatVertexBuffer(glm::value_ptr(data[0]), (unsigned int)data.size(), sizeof(vec2));
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<vec3> >(const std::vector<vec3>& data)
{
	return CreateFloatVertexBuffer(glm::value_ptr(data[0]), (unsigned int)data.size(), sizeof(vec3));
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<vec4> >(const std::vector<vec4>& data)
{
	return CreateFloatVertexBuffer(glm::value_ptr(data[0]), (unsigned int)data.size(), sizeof(vec4));
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateIndexBuffer< std::vector<unsigned int> >(const std::vector<unsigned int>& data)
{
	return CreateUIntIndexBuffer(&(data[0]), (unsigned int)data.size());
}

void RenderDevice::OnLoadingProgress(ProgressEventArgs& e)
{
	LoadingProgress(e);
}