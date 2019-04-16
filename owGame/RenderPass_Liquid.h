#pragma once

// A pass that renders the opaque geometry in the scene.
class CRenderPass_Liquid : public BasePass
{
public:
	typedef BasePass base;

	CRenderPass_Liquid(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~CRenderPass_Liquid();

    // IVisitor
	bool Visit(IMesh& mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override;

protected:

private:
};