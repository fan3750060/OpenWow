#include "stdafx.h"

// General
#include "RenderPass_M2.h"

// Additional

#include "M2_Part_Color.h"
#include "M2_Part_Material.h"
#include "M2_Part_Texture.h"
#include "M2_Part_TextureTransform.h"
#include "M2_Part_TextureWeight.h"

#include "M2_Skin_Batch.h"

CRenderPass_M2::CRenderPass_M2(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: BasePass(scene, pipeline)
{}

CRenderPass_M2::~CRenderPass_M2()
{}



//
// IVisitor
//
bool CRenderPass_M2::Visit(SceneNode3D & node)
{
    return false;
}

bool CRenderPass_M2::Visit(IMesh& mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	const CM2_Skin_Batch* pMesh = dynamic_cast<const CM2_Skin_Batch*>(&mesh);
	if (pMesh)
	{
		//GetPipelineState()->GetBlendState().SetBlendMode(pMesh->GetM2Material()->GetBlendMode());
		//GetPipelineState()->GetBlendState().Bind();

		//GetPipelineState()->GetDepthStencilState().SetDepthMode(pMesh->GetM2Material()->GetDepthMode());
		//GetPipelineState()->GetDepthStencilState().Bind();

		//GetPipelineState()->GetRasterizerState().SetCullMode(pMesh->GetM2Material()->GetCullMode());
		//GetPipelineState()->GetRasterizerState().Bind();

		//GetPipelineState()->Bind();
	}


	if (mesh.GetType() == SN_TYPE_M2)
	{
		return mesh.Render(GetRenderEventArgs(), GetPerObjectConstantBuffer(), IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
	}

	return false;
}
