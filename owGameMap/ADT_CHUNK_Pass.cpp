#include "stdafx.h"

// General
#include "ADT_CHUNK_Pass.h"

ADT_CHUNK_Pass::ADT_CHUNK_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

ADT_CHUNK_Pass::~ADT_CHUNK_Pass()
{}

void ADT_CHUNK_Pass::Visit(Mesh& mesh)
{
	if (mesh.GetType() == SN_TYPE_ADT_CHUNK)
	{
		mesh.Render(GetRenderEventArgs());
	}
}
