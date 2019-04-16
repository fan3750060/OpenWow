#include "stdafx.h"

// General
#include "RenderPass_ADT_MCNK.h"

// Additional
#include "ADT_MCNK.h"

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
bool CRenderPass_ADT_MCNK::Visit(std::shared_ptr<SceneNode3D> node)
{
    std::shared_ptr<ADT_MCNK> adtMCNKInstance = std::dynamic_pointer_cast<ADT_MCNK, SceneNode3D>(node);
    if (adtMCNKInstance)
    {
        return base::Visit(node);
    }

    return false;
}

bool CRenderPass_ADT_MCNK::Visit(std::shared_ptr<IMesh> Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	return Mesh->Render(GetRenderEventArgs(), GetPerObjectConstantBuffer(), IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
}
