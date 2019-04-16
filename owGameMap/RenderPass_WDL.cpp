#include "stdafx.h"

// General
#include "RenderPass_WDL.h"

// Additional
#include "WDL_LowResTile.h"

CRenderPass_WDL::CRenderPass_WDL(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

CRenderPass_WDL::~CRenderPass_WDL()
{}



//
// IVisitor
//
bool CRenderPass_WDL::Visit(std::shared_ptr<SceneNode3D> node)
{
    return base::Visit(node);
}

bool CRenderPass_WDL::Visit(std::shared_ptr<IMesh> Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
    std::shared_ptr<CWDL_LowResTile> wdlMesh = std::dynamic_pointer_cast<CWDL_LowResTile, IMesh>(Mesh);
	if (wdlMesh)
	{
		return wdlMesh->Render(GetRenderEventArgs(), GetPerObjectConstantBuffer(), IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
	}

	return false;
}
