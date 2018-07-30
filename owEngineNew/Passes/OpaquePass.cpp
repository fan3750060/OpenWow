#include <stdafx.h>

// General
#include "OpaquePass.h"

OpaquePass::OpaquePass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

OpaquePass::~OpaquePass()
{}

void OpaquePass::Visit(Mesh& mesh)
{
	std::shared_ptr<Material> pMaterial = mesh.GetMaterial();

	if (pMaterial && !pMaterial->IsTransparent())
	{
		mesh.Render(GetRenderEventArgs());
	}
}
