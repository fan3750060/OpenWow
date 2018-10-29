#pragma once

#include "Mesh.h"

class MeshWrapper : public IMesh 
{
protected:
	typedef MeshWrapper base;
public:
	MeshWrapper(std::shared_ptr<IMesh> _mesh);
	virtual ~MeshWrapper();

	virtual SceneNodeTypes GetType() const;
	virtual void SetType(SceneNodeTypes type);

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<Buffer> buffer);
	virtual void SetIndexBuffer(std::shared_ptr<Buffer> buffer);

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual void SetMaterial(std::shared_ptr<const Material> material);
	virtual std::shared_ptr<const Material> GetMaterial() const;

	virtual void Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, INT baseVertexLocation = 0) override;

	virtual void Accept(IVisitor& visitor);

private:
	const std::shared_ptr<IMesh> m_Mesh;
};