#include "stdafx.h"

// Include
#include "MDX.h"

// General
#include "M2_Skin.h"

CM2_Skin::CM2_Skin(MDX* _model) :
	m_MDX(_model),
	__geom(nullptr)
{}

CM2_Skin::~CM2_Skin()
{}

//

void CM2_Skin::Draw()
{


    _Render->TechniquesMgr()->m_Model->SetAnimated(m_MDX->m_HasBones && m_MDX->m_IsAnimated);
    if (m_MDX->m_HasBones && m_MDX->m_IsAnimated)
    {
        //_Render->TechniquesMgr()->m_Model->SetBoneStartIndex(p->bonesStartIndex); FIXME
        //_Render->TechniquesMgr()->m_Model->SetBoneMaxCount(p->boneInfluences);

        vector<mat4> bones;
        for (uint32 i = 0; i < m_MDX->m_Header.bones.size; i++)
        {
            bones.push_back(m_MDX->m_Bones[i].getTransformMatrix());
        }
        _Render->TechniquesMgr()->m_Model->SetBones(bones);
    }

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
			_Render->TechniquesMgr()->m_Model->SetBillboard(m_MDX->m_IsBillboard);

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
