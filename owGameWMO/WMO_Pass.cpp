#include "stdafx.h"

// General
#include "WMO_Pass.h"

WMO_Pass::WMO_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

WMO_Pass::~WMO_Pass()
{}

void WMO_Pass::Visit(Mesh& mesh)
{
	if (mesh.GetType() == SN_TYPE_WMO)
	{
		mesh.Render(GetRenderEventArgs(), GetConstantBuffer());
	}
}
