#pragma once

class Sky_Pass : public BasePass
{
public:
	typedef BasePass base;

	Sky_Pass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~Sky_Pass();

	virtual bool Visit(IMesh& mesh, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) override;

protected:

private:
};