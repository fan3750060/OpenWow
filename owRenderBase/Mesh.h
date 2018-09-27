#pragma once

#include "BufferBinding.h"

class Buffer;
class Shader;
class Material;
class RenderEventArgs;
class Visitor;

class Mesh : public Object
{
public:
	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<Buffer> buffer) = 0;
	virtual void SetIndexBuffer(std::shared_ptr<Buffer> buffer) = 0;

	virtual void SetMaterial(std::shared_ptr<Material> material) = 0;
	virtual std::shared_ptr<Material> GetMaterial() const = 0;

	virtual void Render(RenderEventArgs& renderEventArgs) = 0;

	virtual void Accept(IVisitor& visitor) = 0;
};

typedef std::shared_ptr<Mesh> SharedMeshPtr;
