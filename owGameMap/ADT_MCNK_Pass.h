#pragma once

// A pass that renders the opaque geometry in the scene.
class ADT_MCNK_Pass : public BasePass
{
public:
	ADT_MCNK_Pass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~ADT_MCNK_Pass();

	virtual bool Visit(IMesh& mesh, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) override;

protected:

private:
	CGroupQuality&						m_QualitySettings;
};