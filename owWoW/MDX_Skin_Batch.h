#pragma once

class MDX_Skin_Batch
{
public:
    uint16 m2SkinIndex;

	int16    __colorIndex;
	uint16    __textureWeight;
	int16     __textureAnims;
	int       __blendMode;
	Material  __material;

	SM2_SkinSection m_SkinSection;
};
