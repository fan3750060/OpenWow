#pragma once

#include "M2_Headers.h"



class CM2_MeshPartID_Provider
{
public:
	enum MeshIDType
	{
		SkinAndHair = 0,  //0: Skin // Hair {1 - 21: various hairstyles}
		Facial1 = 1,  // Facial1 : {1 - 8: varies} (usually beard, but not always)
		Facial2 = 2,  // Facial2 : {1: none(DNE), 2 - 6 : varies} (usually mustache, but not always)
		Facial3 = 3,  // Facial3 : {1: none(DNE), 2 - 11 : varies} (usually sideburns, but not always)
		Glove = 4,  // Glove : {1 - 4}
		Boots = 5,  // Boots : {1 - 5}
		Ears = 7,  // Ears : {1: none(DNE), 2 : ears}
		Wristbands = 8,  // Wristbands / Sleeves : {1: none(DNE), 2 : normal, 3 : ruffled}
		Kneepads = 9,  // Kneepads / Legcuffs : {1: none(DNE), 2 : long, 3 : short}
		Chest = 10, // Chest : {1: none(DNE), 2 : ? ? ? (exists but purpose unknown)}
		Pants = 11, // Pants : {1: regular, 2 : short skirt, 4 : armored pants}
		Tabard = 12, // Tabard : {1: none(DNE), 2 : tabard}
		Trousers = 13, // Trousers : {1: legs, 2 : dress}
		Cloak = 15, // Cloak : {1 - 10: various cloak styles}
		Eyeglows = 17, // Eyeglows : {1: none(DNE), 2 : racial eyeglow, 3 : DK eyeglow}
		Belt = 18, // Belt / bellypack : {1: none(sometimes DNE), 2 : bulky belt}

		Count = 19
	};

public:
	CM2_MeshPartID_Provider();

	void setSpecialTexture(SM2_Texture::SM2_Texture_Type _type, cstring _name);
	R_Texture* getSpecialTexture(SM2_Texture::SM2_Texture_Type _type) const;

	//--

	bool isMeshEnabled(uint32 _index);

	//--

	void setHairGeoset(uint32 _value) { m_MeshID[SkinAndHair] = _value; }
	uint32 getHairGeoset() const { return m_MeshID[SkinAndHair]; }

	void setFacial1Geoset(uint32 _value) { m_MeshID[Facial1] = _value; }
	uint32 getFacial1Geoset() const { return m_MeshID[Facial1]; }

	void setFacial2Geoset(uint32 _value) { m_MeshID[Facial2] = _value; }
	uint32 getFacial2Geoset() const { return m_MeshID[Facial2]; }

	void setFacial3Geoset(uint32 _value) { m_MeshID[Facial3] = _value; }
	uint32 getFacial3Geoset() const { return m_MeshID[Facial3]; }

private:
	R_Texture* m_SpecialTextures[SM2_Texture::SM2_Texture_Type::TEX_COMPONENT_COUNT];

	uint32 m_MeshID[MeshIDType::Count + 10];
};