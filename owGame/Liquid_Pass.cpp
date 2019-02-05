#include "stdafx.h"

// General
#include "Liquid_Pass.h"

Liquid_Pass::Liquid_Pass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

Liquid_Pass::~Liquid_Pass()
{}

bool Liquid_Pass::Visit(IMesh& mesh)
{
	if (mesh.GetType() == SN_TYPE_LQ)
	{
		return mesh.Render(GetRenderEventArgs(), GetPerObjectConstantBuffer());
	}

	return false;
}
