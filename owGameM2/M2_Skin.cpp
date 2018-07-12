#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_Skin.h"

// Additional
#include "M2_Part_Material.h"

CM2_Skin::CM2_Skin(M2* _model) :
	m_ParentM2(_model),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{}

CM2_Skin::~CM2_Skin()
{
	ERASE_VECTOR(m_Batches);
}

//

void CM2_Skin::Draw(CM2_MeshPartID_Provider* _provider, uint16 _animationIndex, cmat4 _worldMatrix, uint32 _time, uint32 globalTime)
{
	for (auto& p : m_Batches)
	{
		p->Render(_provider, _animationIndex, _worldMatrix, _time, globalTime);
	}
}


/*void CM2_Skin::RenderNormals(cmat4 _worldMatrix)
{
	_Render->getTechniquesMgr()->DebugNormal_Pass->Bind();
	_Render->getTechniquesMgr()->DebugNormal_Pass->setWorld(_worldMatrix);

	_Render->r.setGeometry(__geomDebugNormals);

	for (auto p : m_Batches)
	{
		if (m_VisibleSubmeshes[p->m2SkinIndex])
		{
			_Render->r.drawIndexed
			(
				PRIM_TRILIST,
				p->m_SkinProtoSection.indexStart,
				p->m_SkinProtoSection.indexCount,
				p->m_SkinProtoSection.vertexStart,
				p->m_SkinProtoSection.vertexCount,
				nullptr,
				false
			);
		}
	}

	_Render->getTechniquesMgr()->DebugNormal_Pass->Unbind();
}*/
