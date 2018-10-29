#pragma once

#include "RenderEvents.h"
#include "CPUAccess.h"

class Buffer;
class ConstantBuffer;
class StructuredBuffer;
#include "Mesh.h"
#include "MeshWrapper.h"
#include "Shader.h"
#include "Texture.h"
class RenderTarget;
#include "Query.h"
class SamplerState;
#include "Material.h"
class PipelineState;

/**
 * Render device encapsulates functions for creating resources on the GPU.
 */
class RenderDevice : public Object
{
public:
	// Get the name of the primary graphics device.
	virtual cstring GetDeviceName() const = 0;

	// Create an vertex buffer (with std::vector)
	template<typename T>
	std::shared_ptr<Buffer> CreateVertexBuffer(const T& data);
	template<typename T>
	std::shared_ptr<Buffer> CreateVertexBuffer(const T* data, uint32 count);
	virtual void DestroyVertexBuffer(std::shared_ptr<Buffer> buffer) = 0;

	// Create an index buffer.
	template<typename T>
	std::shared_ptr<Buffer> CreateIndexBuffer(const T& data);
	template<typename T>
	std::shared_ptr<Buffer> CreateIndexBuffer(const T* data, uint32 count);
	virtual void DestroyIndexBuffer(std::shared_ptr<Buffer> buffer) = 0;

	// Create a constant buffer (or Uniform buffer)
	template<typename T>
	std::shared_ptr<ConstantBuffer> CreateConstantBuffer(const T& data);
	virtual void DestroyConstantBuffer(std::shared_ptr<ConstantBuffer> buffer) = 0;

	// Create a StructuredBuffer
	template<typename T>
	std::shared_ptr<StructuredBuffer> CreateStructuredBuffer(const std::vector<T>& data, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	virtual void DestroyStructuredBuffer(std::shared_ptr<StructuredBuffer> buffer) = 0;

	// Create a plane in 3D.
	// The plane will be centered at the origin.
	// @param size The size of the plane.
	// @param N Surface normal to the plane.
	std::shared_ptr<IMesh> CreatePlane(cvec3 N = vec3(0, 1, 0));

	// Create a screen-space quad that can be used to render full-screen post-process effects to the screen.
	// By default, the quad will have clip-space coordinates and can be used with a pass-through vertex shader
	// to render full-screen post-process effects. If you want more control over the area of the screen the quad covers, 
	// you can specify your own screen coordinates and supply an appropriate orthographic projection matrix to align the 
	// screen quad appropriately.
	std::shared_ptr<IMesh> CreateScreenQuad(float left = -1.0f, float right = 1.0f, float bottom = -1.0f, float top = 1.0f, float z = 0.0f);

	// Create a sphere in 3D
	// @param radius Radius of the sphere.
	// @param tesselation The amount of tessellation to apply to the sphere. Default tessellation is 4.
	std::shared_ptr<IMesh> CreateSphere();

	// Create a cube in 3D.
	// The cube will be centered at the origin.
	// @param size The length of each edge of the cube.
	std::shared_ptr<IMesh> CreateCube();

	// Create a cylinder that is aligned to a particular axis.
	// @param baseRadius The radius of the base (bottom) of the cylinder.
	// @param apexRadius The radius of the apex (top) of the cylinder.
	// @param height The height of the sphere along the axis of the cylinder.
	// @param axis The axis to align the cylinder. Default to the global Y axis.
	std::shared_ptr<IMesh> CreateCylinder(float baseRadius, float apexRadius, float height, cvec3 axis = vec3(0, 1, 0));

	// Create a cone.
	// Cones are always aligned to (0, 1, 0) with the base of the cone 
	// centered at (0, 0, 0) and apex at (0, height, 0).
	// A cone is just a cylinder with an apex radius of 0.
	// @param baseRadius The radius of the base of the cone.
	// @param height The height of the cone.
	std::shared_ptr<IMesh> CreateCone();

	// Create a 3D arrow.
	// Arrows can be used to represent the direction an object or light is pointing.
	// @param tail The tail (begin point) of the arrow.
	// @param head The head (end point) of the arrow.
	// @param radius The radius of the body of the arrow.
	std::shared_ptr<IMesh> CreateArrow(cvec3 tail = vec3(0, 0, 0), cvec3 head = vec3(0, 0, 1), float radius = 0.05f);

	// Create a 3D axis with X, -X, Y, -Y, Z, -Z axes.
	// Primarily used to debug an object's position and direction in 3D space.
	// The axis is aligned to 0,0,0 and the global X, Y, Z axes.
	// @param radius is the radius of the axis arms.
	// @param length is the length is the length of each axis arm.
	std::shared_ptr<IMesh> CreateAxis(float radius = 0.05f, float length = 0.5f);

	//
	// ABSTRACT
	//

	virtual std::shared_ptr<IMesh> CreateMesh() = 0;
	virtual void DestroyMesh(std::shared_ptr<IMesh> mesh) = 0;

	virtual std::shared_ptr<Shader> CreateShader(Shader::ShaderType type, cstring fileName, const Shader::ShaderMacros& shaderMacros, cstring entryPoint, cstring profile) = 0;
	virtual void DestroyShader(std::shared_ptr<Shader> shader) = 0;

	// Create a texture from a file.
	virtual std::shared_ptr<Texture> CreateTexture2D(cstring fileName) = 0;
	virtual std::shared_ptr<Texture> CreateTextureCube(cstring fileName) = 0;

	// Create an empty texture of a predefined size.
	virtual std::shared_ptr<Texture> CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices, const Texture::TextureFormat& format = Texture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;
	virtual std::shared_ptr<Texture> CreateTextureCube(uint16_t size, uint16_t numCubes = 1, const Texture::TextureFormat& format = Texture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;
	virtual std::shared_ptr<Texture> GetDefaultTexture() const = 0;

	// Create an null texture (can be loaded later using Texture::LoadTexture2D function.
	virtual std::shared_ptr<Texture> CreateTexture() = 0;
	virtual void DestroyTexture(std::shared_ptr<Texture> texture) = 0;

	// Create a render target
	virtual std::shared_ptr<RenderTarget> CreateRenderTarget() = 0;
	virtual void DestroyRenderTarget(std::shared_ptr<RenderTarget> renderTarget) = 0;

	// Create a GPU query object. Used for performance profiling, occlusion queries,
	// or primitive output queries.
	virtual std::shared_ptr<Query> CreateQuery(Query::QueryType queryType = Query::QueryType::Timer, uint8_t numBuffers = 3) = 0;
	virtual void DestoryQuery(std::shared_ptr<Query> query) = 0;

	virtual std::shared_ptr<SamplerState> CreateSamplerState() = 0;
	virtual void DestroySampler(std::shared_ptr<SamplerState> sampler) = 0;

	virtual std::shared_ptr<PipelineState> CreatePipelineState() = 0;
	virtual void DestoryPipelineState(std::shared_ptr<PipelineState> pipeline) = 0;

	virtual std::shared_ptr<Buffer> CreateFloatVertexBuffer(const float* data, uint32 count, uint32 offset, uint32 stride) = 0;
	virtual std::shared_ptr<Buffer> CreateDoubleVertexBuffer(const double* data, uint32 count, uint32 offset, uint32 stride) = 0;
	virtual std::shared_ptr<Buffer> CreateUInt16IndexBuffer(const uint16* data, uint32 sizeInBytes) = 0;
	virtual std::shared_ptr<Buffer> CreateUInt32IndexBuffer(const uint32* data, uint32 sizeInBytes) = 0;
	virtual std::shared_ptr<ConstantBuffer> CreateConstantBuffer(const void* data, size_t size) = 0;
	virtual std::shared_ptr<StructuredBuffer> CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;

	// Wrappers
	std::shared_ptr<MeshWrapper> CreateMeshWrapper();
};

#include "RenderDevice.inl"