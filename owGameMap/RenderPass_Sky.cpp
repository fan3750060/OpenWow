#include "stdafx.h"

// General
#include "RenderPass_Sky.h"

CRenderPass_Sky::CRenderPass_Sky(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

CRenderPass_Sky::~CRenderPass_Sky()
{}



//
// IVisitor
//
bool CRenderPass_Sky::Visit(IMesh& Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	if (Mesh.GetType() == SN_TYPE_SKY)
	{
		return Mesh.Render(GetRenderEventArgs(), GetPerObjectConstantBuffer(), IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
	}

	return false;
}
