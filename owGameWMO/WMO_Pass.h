#pragma once

// A pass that renders the opaque geometry in the scene.
class WMO_Pass : public BasePass
{
public:
	typedef BasePass base;

	WMO_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~WMO_Pass();

	virtual void Visit(IMesh& mesh);

protected:

private:
};