#pragma once

// A pass that renders the opaque geometry in the scene.
class WDL_Node_Pass : public BasePass
{
public:
	typedef BasePass base;

	WDL_Node_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~WDL_Node_Pass();

	virtual bool Visit(IMesh& mesh) override;

protected:

private:
};