#include "stdafx.h"

// General
#include "WMO_Pass.h"

WMO_Pass::WMO_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

WMO_Pass::~WMO_Pass()
{}

bool WMO_Pass::Visit(IMesh& mesh)
{
	if (mesh.GetType() == SN_TYPE_WMO)
	{
		return mesh.Render(GetRenderEventArgs(), GetConstantBuffer());
	}

	return false;
}
