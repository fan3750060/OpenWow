#include <stdafx.h>

// General
#include "OpaquePass.h"

OpaquePass::OpaquePass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

OpaquePass::~OpaquePass()
{}

void OpaquePass::Visit(IMesh& mesh)
{
	std::shared_ptr<const Material> pMaterial = mesh.GetMaterial();

	if (pMaterial /*&& !pMaterial->IsTransparent()*/)
	{
		//mesh.Render(GetRenderEventArgs());
	}
}
