#include "stdafx.h"

// General
#include "Sky_Pass.h"

Sky_Pass::Sky_Pass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

Sky_Pass::~Sky_Pass()
{}

bool Sky_Pass::Visit(IMesh& mesh)
{
	if (mesh.GetType() == SN_TYPE_SKY)
	{
		return mesh.Render(GetRenderEventArgs(), GetPerObjectConstantBuffer());
	}

	return false;
}
