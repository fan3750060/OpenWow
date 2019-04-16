#pragma once

// A pass that renders the opaque geometry in the scene.
class CRenderPass_ADT_MCNK : public BasePass
{
public:
	CRenderPass_ADT_MCNK(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~CRenderPass_ADT_MCNK();

    // IVisitor
	bool Visit(IMesh& Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override final;

protected:

private:
	CGroupQuality&						m_QualitySettings;
};