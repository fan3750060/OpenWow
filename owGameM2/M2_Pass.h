#pragma once

// A pass that renders the opaque geometry in the scene.
class M2_Pass : public BasePass
{
public:
	typedef BasePass base;

	M2_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~M2_Pass();

	virtual void Visit(IMesh& mesh);

protected:

private:
};