#pragma once

class CRenderPass_WMO : public BasePass
{
    typedef BasePass base;
public:
	CRenderPass_WMO(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~CRenderPass_WMO();

    // IVisitor
    bool Visit(std::shared_ptr<SceneNode3D> node) override final;
	bool Visit(std::shared_ptr<IMesh>, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override final;
};