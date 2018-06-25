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

private:
	const M2*							m_ParentM2;

	uint32								m_SkinIndex;
	SM2_SkinSection						m_SkinSection;

	const CM2_Part_Color*				color; 
	const CM2_Part_Material*			material;
	vector<const CM2_Part_Texture*>		m_Textures;
	int16								texture_Unit;
	const CM2_Part_TextureWeight*		texture_WeightIndex;
	const CM2_Part_TextureTransform*	texture_Transform;

	bool								m_IsBilldoard;

	int32 newShader;


	CGroupQuality& m_QualitySettings;
};
