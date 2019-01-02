#include "stdafx.h"

// General
#include "M2_Pass.h"

M2_Pass::M2_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline)
	: BasePass(scene, pipeline)
{}

M2_Pass::~M2_Pass()
{}

void M2_Pass::Visit(IMesh& mesh)
{
	if (mesh.GetType() == SN_TYPE_M2)
	{
		mesh.Render(GetRenderEventArgs(), GetConstantBuffer());
	}
}
