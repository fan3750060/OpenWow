#pragma once

#include "M2_Part_Material.h"

// FORWARD BEGIN
class CM2_Skin_Builder;
// FORWARD END

class M2_Skin_Batch
{
	friend CM2_Skin_Builder;
public:
	M2_Skin_Batch(const M2* _parentM2);

	void Render();

	int32 getPriorityPlan() const { return m_PriorityPlan; }
	uint32 getSkinProtoIndex() const { return m_SkinProtoIndex; }
	const SM2_SkinSection& getSkinProto() const { return m_SkinProtoSection; }

private:
	int32								m_PriorityPlan;
	uint32								m_SkinProtoIndex;
	SM2_SkinSection						m_SkinProtoSection;

	const CM2_Part_Color*				color;
	const CM2_Part_Material*			material;
	vector<const CM2_Part_Texture*>		m_Textures;
	int16								texture_Unit;
	const CM2_Part_TextureWeight*		texture_WeightIndex;
	const CM2_Part_TextureTransform*	texture_Transform;

	bool								m_IsBilldoard;

	int32 newShader;

	//--
	const M2*							m_ParentM2;
	CGroupQuality&						m_QualitySettings;
};
