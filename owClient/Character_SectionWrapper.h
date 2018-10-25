#pragma once

// FORWARD BEGIN
class Character;
// FORWARD END

class Character_SectionWrapper
{
public:
	// Skin
	static std::shared_ptr<Texture> getSkinTexture(Character* _character);
	static std::shared_ptr<Texture> getSkinExtraTexture(Character* _character);

	// Face
	static std::shared_ptr<Texture> getFaceLowerTexture(Character* _character);
	static std::shared_ptr<Texture> getFaceUpperTexture(Character* _character);

	// FacialHair
	static std::string getFacialHairLowerTexture(Character* _character);
	static std::string getFacialHairUpperTexture(Character* _character);
	static uint32 getFacial1Geoset(Character* _character);
	static uint32 getFacial2Geoset(Character* _character);
	static uint32 getFacial3Geoset(Character* _character);

	// Hair
	static uint32     getHairGeoset(Character* _character);
	static std::shared_ptr<Texture> getHairTexture(Character* _character);
	static std::shared_ptr<Texture> getHairScalpLowerTexture(Character* _character);
	static std::shared_ptr<Texture> getHairScalpUpperTexture(Character* _character);

	// Naked
	static std::string getNakedPelvisTexture(Character* _character);
	static std::string getNakedTorsoTexture(Character* _character);
};