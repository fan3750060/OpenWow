#pragma once

// A pass that renders the opaque geometry in the scene.
class UI_Pass : public BaseUIPass
{
	typedef BaseUIPass base;
public:
	UI_Pass(std::shared_ptr<UIScene> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~UI_Pass();

	virtual bool Visit(IMesh& mesh) override;

protected:

private:
};