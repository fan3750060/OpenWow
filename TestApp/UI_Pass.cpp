#include "stdafx.h"

// General
#include "UI_Pass.h"

UI_Pass::UI_Pass(std::shared_ptr<UIScene> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

UI_Pass::~UI_Pass()
{}

bool UI_Pass::Visit(IMesh& mesh)
{
	RenderEventArgs args(GetRenderUIEventArgs().Caller, GetRenderUIEventArgs().ElapsedTime, GetRenderUIEventArgs().TotalTime, GetRenderUIEventArgs().FrameCounter);
	return mesh.Render(args, GetPerObjectConstantBuffer());
}
