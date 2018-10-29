#pragma once

#include "BufferBinding.h"
#include "PrimitiveTopology.h"

class Buffer;
class Shader;
class Material;
class RenderEventArgs;
class IVisitor;
class ConstantBuffer;

enum SceneNodeTypes : uint8
{
	SN_TYPE_NONE = 0,
	SN_TYPE_WDL_NODE,
	SN_TYPE_ADT_CHUNK,
	SN_TYPE_WMO
};

class IMesh : public Object
{
public:
	IMesh();
	virtual ~IMesh();

	virtual SceneNodeTypes GetType() const;
	virtual void SetType(SceneNodeTypes type);

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<Buffer> buffer) = 0;
	virtual void SetIndexBuffer(std::shared_ptr<Buffer> buffer) = 0;

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology) = 0;

	virtual void SetMaterial(std::shared_ptr<const Material> material) = 0;
	virtual std::shared_ptr<const Material> GetMaterial() const = 0;

	virtual void Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, INT baseVertexLocation = 0) = 0;

	virtual void Accept(IVisitor& visitor) = 0;

private:
	SceneNodeTypes m_Type;
};
