#include "stdafx.h"

// General
#include "WDL_Node_Pass.h"

WDL_Node_Pass::WDL_Node_Pass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

WDL_Node_Pass::~WDL_Node_Pass()
{}

bool WDL_Node_Pass::Visit(IMesh& mesh)
{
	if (mesh.GetType() == SN_TYPE_WDL_NODE)
	{
		return mesh.Render(GetRenderEventArgs(), GetPerObjectConstantBuffer());
	}

	return false;
}
