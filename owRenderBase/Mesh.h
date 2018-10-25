#pragma once

#include "BufferBinding.h"
#include "PrimitiveTopology.h"

class Buffer;
class Shader;
class Material;
class RenderEventArgs;
class Visitor;
class ConstantBuffer;

enum SceneNodeTypes : uint8
{
	SN_TYPE_NONE = 0,
	SN_TYPE_WDL_NODE,
	SN_TYPE_ADT_CHUNK,
	SN_TYPE_WMO
};

class Mesh : public Object
{
public:
	Mesh();

	virtual SceneNodeTypes GetType() const;
	virtual void SetType(SceneNodeTypes type);

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<Buffer> buffer) = 0;
	virtual void SetIndexBuffer(std::shared_ptr<Buffer> buffer) = 0;

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology) = 0;

	virtual void SetMaterial(std::shared_ptr<Material> material) = 0;
	virtual std::shared_ptr<Material> GetMaterial() const = 0;

	virtual void Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> constantBuffer) = 0;

	virtual void Accept(IVisitor& visitor) = 0;

private:
	SceneNodeTypes m_Type;
};
