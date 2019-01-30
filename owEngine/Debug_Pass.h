#pragma once

#include "BasePass.h"

// A pass that renders the opaque geometry in the scene.
class Debug_Pass : public BasePass
{
public:
	Debug_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~Debug_Pass();

	virtual bool Visit(IMesh& mesh);

protected:

private:
};