#pragma once

struct MDX_Skin_Batch
{
	MDX_Skin_Batch() :
		m_IsBilldoard(false)
	{}

    uint16 m2SkinIndex;

	int16    __colorIndex;
	uint16    __textureWeight;
	int16     __textureAnims;
	int       __blendMode;
	Material  __material;

	bool m_IsBilldoard;

	SM2_SkinSection m_SkinSection;
};
