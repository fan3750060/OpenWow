#pragma once

#include "M2_Part_Material.h"

// FORWARD BEGIN
class M2;
class CM2_Skin;
class CM2_Skin_Builder;
// FORWARD END

class CM2_Skin_Batch
{
	friend CM2_Skin_Builder;
public:
	CM2_Skin_Batch(const M2* _parentM2, const CM2_Skin* _parentSkin);

	void Init();

	void Render(CM2_MeshPartID_Provider* _provider);

	int32 getPriorityPlan() const { return m_PriorityPlan; }
	uint32 getSkinProtoIndex() const { return m_SkinProtoIndex; }
	const SM2_SkinSection& getSkinProto() const { return m_SkinProtoSection; }

private:
	int32								m_PriorityPlan;
	uint32								m_SkinProtoIndex;
	SM2_SkinSection						m_SkinProtoSection;

	const CM2_Part_Color*				m_Color;
	const CM2_Part_Material*			m_Material;
	vector<const CM2_Part_Texture*>		m_Textures;
	int16								m_TextureUnit;
	const CM2_Part_TextureWeight*		m_TextureWeight;
	const CM2_Part_TextureTransform*	m_TextureTransform;

	int32								newShader;

	RenderState							m_State;

	//--
	const M2*							m_ParentM2;
	const CM2_Skin*						m_ParentSkin;
	CGroupQuality&						m_QualitySettings;
};
