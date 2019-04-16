#pragma once

class CRenderPass_Sky : public BasePass
{
    typedef BasePass base;
public:
	CRenderPass_Sky(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~CRenderPass_Sky();

    // IVisitor
	bool Visit(IMesh& Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override final;

protected:

private:
};