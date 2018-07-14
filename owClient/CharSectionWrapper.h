#pragma once

// FORWARD BEGIN
class Character;
// FORWARD END

class CharSectionWrapper
{
public:
	// Skin
	static string getSkinTexture(Character* _character);

	// Face
	static string getFaceLowerTexture(Character* _character);
	static string getFaceUpperTexture(Character* _character);

	// FacialHair
	static string getFacialHairLowerTexture(Character* _character);
	static string getFacialHairUpperTexture(Character* _character);
	static uint32 getFacial1Geoset(Character* _character);
	static uint32 getFacial2Geoset(Character* _character);
	static uint32 getFacial3Geoset(Character* _character);

	// Hair
	static string getHairTexture(Character* _character);
	static string getHairScalpLowerTexture(Character* _character);
	static string getHairScalpUpperTexture(Character* _character);
	static uint32 getHairGeoset(Character* _character);
};