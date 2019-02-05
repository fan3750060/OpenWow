#include "stdafx.h"

// General
#include "M2_Pass.h"

// Additional

#include "M2_Part_Color.h"
#include "M2_Part_Material.h"
#include "M2_Part_Texture.h"
#include "M2_Part_TextureTransform.h"
#include "M2_Part_TextureWeight.h"

#include "M2_Skin_Batch.h"

M2_Pass::M2_Pass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: BasePass(scene, pipeline)
{}

M2_Pass::~M2_Pass()
{}

bool M2_Pass::Visit(IMesh& mesh)
{
	const CM2_Skin_Batch* pMesh = dynamic_cast<const CM2_Skin_Batch*>(&mesh);
	if (pMesh)
	{
		GetPipelineState()->GetBlendState().SetBlendMode(pMesh->GetM2Material()->GetBlendMode());
		GetPipelineState()->GetBlendState().Bind();

		GetPipelineState()->GetDepthStencilState().SetDepthMode(pMesh->GetM2Material()->GetDepthMode());
		GetPipelineState()->GetDepthStencilState().Bind();

		GetPipelineState()->GetRasterizerState().SetCullMode(pMesh->GetM2Material()->GetCullMode());
		GetPipelineState()->GetRasterizerState().Bind();

		//GetPipelineState()->Bind();
	}


	if (mesh.GetType() == SN_TYPE_M2)
	{
		return mesh.Render(GetRenderEventArgs(), GetPerObjectConstantBuffer());
	}

	return false;
}
