#include "stdafx.h"

// General
#include "RenderPass_Liquid.h"

CRenderPass_Liquid::CRenderPass_Liquid(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

CRenderPass_Liquid::~CRenderPass_Liquid()
{}



//
// IVisitor
//
bool CRenderPass_Liquid::Visit(IMesh& mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	if (mesh.GetType() == SN_TYPE_LQ)
	{
		return mesh.Render(GetRenderEventArgs(), GetPerObjectConstantBuffer(), IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
	}

	return false;
}
