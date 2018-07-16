#pragma once

#include "M2_Part_Material.h"
#include "M2_SkinSection.h"

// FORWARD BEGIN
class M2;
class CM2_Base_Instance;
class CM2_Skin_Builder;
// FORWARD END

class CM2_Skin_Batch
{
	friend CM2_Skin_Builder;
public:
	CM2_Skin_Batch(const M2* _parentM2);

	void Init();

	void Render(CM2_Base_Instance* _instance);

	int32 getPriorityPlan() const { return m_PriorityPlan; }
	const CM2_SkinSection* getSkin() const { return m_SkinSection; }

private:
	int32								m_PriorityPlan;
	CM2_SkinSection*					m_SkinSection;

	const CM2_Part_Color*				m_Color;
	const CM2_Part_Material*			m_Material;
	vector<const CM2_Part_Texture*>		m_Textures;
	int16								m_TextureUnit;
	const CM2_Part_TextureWeight*		m_TextureWeight;
	const CM2_Part_TextureTransform*	m_TextureTransform;

	int32								newShader;

	RenderState							m_State;

private: // PARENT
	const M2*							m_ParentM2;
	CGroupQuality&						m_QualitySettings;
};
