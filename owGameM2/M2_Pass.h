#pragma once

// A pass that renders the opaque geometry in the scene.
class M2_Pass : public BasePass
{
public:
	M2_Pass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~M2_Pass();

	virtual bool Visit(IMesh& mesh);

protected:

private:
};