// Template specializations for vertex buffers.
template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<float> >(const std::vector<float>& data);
template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<vec2> >(const std::vector<vec2>& data);
template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<vec3> >(const std::vector<vec3>& data);
template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<vec4> >(const std::vector<vec4>& data);

// Template specializations for index buffers.
template<>
std::shared_ptr<Buffer> RenderDevice::CreateIndexBuffer< std::vector<uint16> >(const std::vector<uint16>& data);
template<>
std::shared_ptr<Buffer> RenderDevice::CreateIndexBuffer< std::vector<uint32> >(const std::vector<uint32>& data);

// Non-specialized template methods.
template< typename T >
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer(const T& data)
{
	static_assert(false, "This function must be specialized.");
	return NULL;
}

template<typename T>
std::shared_ptr<Buffer> RenderDevice::CreateIndexBuffer(const T& data)
{
	static_assert(false, "This function must be specialized.");
	return NULL;
}

template< typename T >
std::shared_ptr<ConstantBuffer> RenderDevice::CreateConstantBuffer(const T& data)
{
	return CreateConstantBuffer(&data, sizeof(T));
}

template<typename T>
std::shared_ptr<StructuredBuffer> RenderDevice::CreateStructuredBuffer(const std::vector<T>& data, CPUAccess cpuAccess, bool gpuWrite)
{
	size_t stride = sizeof(T);
	size_t numElements = data.size();
	return CreateStructuredBuffer((void*)data.data(), (uint32)numElements, (uint32)stride, cpuAccess, gpuWrite);
}