#pragma once

class Sky_Pass : public BasePass
{
public:
	typedef BasePass base;

	Sky_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~Sky_Pass();

	virtual bool Visit(IMesh& mesh) override;

protected:

private:
};