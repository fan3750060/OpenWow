#pragma once

// A pass that renders the opaque geometry in the scene.
class ADT_CHUNK_Pass : public BasePass
{
public:
	ADT_CHUNK_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~ADT_CHUNK_Pass();

	virtual bool Visit(IMesh& mesh) override;

protected:

private:
	CGroupQuality&						m_QualitySettings;
};