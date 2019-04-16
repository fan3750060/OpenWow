#pragma once

class CRenderPass_WDL : public BasePass
{
    typedef BasePass base;
public:
	CRenderPass_WDL(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~CRenderPass_WDL();

    // IVisitor
    bool Visit(std::shared_ptr<SceneNode3D> node) override final;
	bool Visit(std::shared_ptr<IMesh> Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override final;
};