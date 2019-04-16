#include "stdafx.h"

// General
#include "RenderPass_WDL.h"

CRenderPass_WDL::CRenderPass_WDL(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

CRenderPass_WDL::~CRenderPass_WDL()
{}



//
// IVisitor
//
bool CRenderPass_WDL::Visit(IMesh& Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	if (Mesh.GetType() == SN_TYPE_WDL_NODE)
	{
		return Mesh.Render(GetRenderEventArgs(), GetPerObjectConstantBuffer(), IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
	}

	return false;
}
