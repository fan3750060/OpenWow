#include "stdafx.h"

// General
#include "RenderPass_Sky.h"

// Additional 
#include "SkyManager.h"

CRenderPass_Sky::CRenderPass_Sky(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{
}

CRenderPass_Sky::~CRenderPass_Sky()
{}



//
// IVisitor
//
bool CRenderPass_Sky::Visit(std::shared_ptr<SceneNode3D> node)
{
    std::shared_ptr<SkyManager> skyManagerInstance = std::dynamic_pointer_cast<SkyManager, SceneNode3D>(node);
    if (skyManagerInstance)
    {
        return base::Visit(node);
    }

    return false;
}

bool CRenderPass_Sky::Visit(std::shared_ptr<IMesh> Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
    return Mesh->Render(GetRenderEventArgs(), GetPerObjectConstantBuffer(), IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
}
