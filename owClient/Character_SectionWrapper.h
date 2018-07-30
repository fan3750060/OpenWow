#pragma once

// FORWARD BEGIN
class Character;
// FORWARD END

class Character_SectionWrapper
{
public:
	// Skin
	static SharedTexturePtr getSkinTexture(Character* _character);
	static SharedTexturePtr getSkinExtraTexture(Character* _character);

	// Face
	static SharedTexturePtr getFaceLowerTexture(Character* _character);
	static SharedTexturePtr getFaceUpperTexture(Character* _character);

	// FacialHair
	static string getFacialHairLowerTexture(Character* _character);
	static string getFacialHairUpperTexture(Character* _character);
	static uint32 getFacial1Geoset(Character* _character);
	static uint32 getFacial2Geoset(Character* _character);
	static uint32 getFacial3Geoset(Character* _character);

	// Hair
	static uint32     getHairGeoset(Character* _character);
	static SharedTexturePtr getHairTexture(Character* _character);
	static SharedTexturePtr getHairScalpLowerTexture(Character* _character);
	static SharedTexturePtr getHairScalpUpperTexture(Character* _character);

	// Naked
	static string getNakedPelvisTexture(Character* _character);
	static string getNakedTorsoTexture(Character* _character);
};