#include "stdafx.h"

// General
#include "RenderPass_ADT_MCNK.h"

CRenderPass_ADT_MCNK::CRenderPass_ADT_MCNK(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: BasePass(scene, pipeline)
	, m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
}

CRenderPass_ADT_MCNK::~CRenderPass_ADT_MCNK()
{}



//
// IVisitor
//
bool CRenderPass_ADT_MCNK::Visit(IMesh& Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	if (Mesh.GetType() == SN_TYPE_ADT_CHUNK && m_QualitySettings.draw_mcnk)
	{
		return Mesh.Render(GetRenderEventArgs(), GetPerObjectConstantBuffer(), IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
	}

	return false;
}
