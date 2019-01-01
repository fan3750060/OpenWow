#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Base_Instance.h"
#include "M2_Skin_Builder.h"

// General
#include "M2_Skin.h"

// Additional
#include "M2_Part_Material.h"

CM2_Skin::CM2_Skin(const std::weak_ptr<const M2> _model) :
	m_ParentM2(_model),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{}

void CM2_Skin::CreateInsances(std::weak_ptr<CM2_Base_Instance> _parent)
{
	for (const auto& batch : m_Batches)
	{
		_parent.lock()->AddMesh(batch);
	}
}

//

void CM2_Skin::Draw(CM2_Base_Instance* _instance)
{
	for (auto p : m_Batches)
	{
		//p->Render(_instance);
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
