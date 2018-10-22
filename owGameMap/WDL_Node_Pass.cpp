#include "stdafx.h"

// General
#include "WDL_Node_Pass.h"

WDL_Node_Pass::WDL_Node_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

WDL_Node_Pass::~WDL_Node_Pass()
{}

void WDL_Node_Pass::Visit(Mesh& mesh)
{
	if (mesh.GetType() == SN_TYPE_WDL_NODE)
	{
		mesh.Render(GetRenderEventArgs());
	}
}
