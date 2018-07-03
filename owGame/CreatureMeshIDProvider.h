#pragma once

class CCreatureMeshIDProvider : public CM2_MeshPartID_Provider
{
public:
	CCreatureMeshIDProvider();

	bool isMeshEnabled(uint32 _index) const override;

	void setSpecialTexture(SM2_Texture::SM2_Texture_Type _type, cstring _name);
	R_Texture* getSpecialTexture(SM2_Texture::SM2_Texture_Type _type) const override;

private:
	R_Texture* m_SpecialTextures[SM2_Texture::SM2_Texture_Type::TEX_COMPONENT_COUNT];
};