#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_Skin.h"

CM2_Skin::CM2_Skin(M2* _model) :
	m_MDX(_model),
	__geom(nullptr)
{}

CM2_Skin::~CM2_Skin()
{}

//

void CM2_Skin::Draw()
{
	_Render->r.setGeometry(__geom);

	for (auto p : m_Batches)
	{
		if (m_VisibleSubmeshes[p->m2SkinIndex])
		{
			p->__material.Set();

		
			// Color
			if (p->__colorIndex != -1)
			{
				_Render->TechniquesMgr()->m_Model->SetColor(m_MDX->m_Colors[p->__colorIndex].getValue());
			}
		    else
			{
				_Render->TechniquesMgr()->m_Model->SetColor(vec4(0.5f, 0.5f, 0.5f, 1.0f));
			}

			// Blend & Alpha
			_Render->TechniquesMgr()->m_Model->SetBlendMode(p->__blendMode);

			// R_Texture weight
			_Render->TechniquesMgr()->m_Model->SetTextureWeight(m_MDX->m_TextureWeights[p->__textureWeight].getValue());

			// Billboard
			_Render->TechniquesMgr()->m_Model->SetBillboard(p->m_IsBilldoard);

			// R_Texture anim
			_Render->TechniquesMgr()->m_Model->SetTextureAnimEnable(p->__textureAnims != -1);
			if (p->__textureAnims != -1)
			{
				_Render->TechniquesMgr()->m_Model->SetTextureAnimMatrix(m_MDX->m_TexturesTransform[p->__textureAnims].getValue());
			}
			else
			{
				//continue;
			}

			_Render->r.drawIndexed
			(
				PRIM_TRILIST,
				p->m_SkinSection.indexStart,
				p->m_SkinSection.indexCount,
				p->m_SkinSection.vertexStart,
				p->m_SkinSection.vertexCount,
				false
			);
		}
	}


}
