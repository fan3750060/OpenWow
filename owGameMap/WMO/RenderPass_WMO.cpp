#include "stdafx.h"

// General
#include "RenderPass_WMO.h"

CRenderPass_WMO::CRenderPass_WMO(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

CRenderPass_WMO::~CRenderPass_WMO()
{}



//
// IVisitor
//
bool CRenderPass_WMO::Visit(SceneNode3D& node)
{
    return false; // base::Visit(node);
}

bool CRenderPass_WMO::Visit(IMesh& Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	if (Mesh.GetType() == SN_TYPE_WMO)
	{
		return Mesh.Render(GetRenderEventArgs(), GetPerObjectConstantBuffer(), IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
	}

	return false;
}
