#pragma once

struct CharacterSkinLayout
{
	enum List : uint32
	{
		LAYOUT_1 = 1,
		LAYOUT_2 = 2
	};
};

// FORWARD BEGIN
class Character;
// FORWARD END

class Character_SkinTextureBaker
{
public:
	Character_SkinTextureBaker();

	R_Texture* createTexture(Character* _character);

private:
	void FillWithSkin(R_Texture* _texture);
	void FillPixels(DBC_CharComponent_Sections::List _type, string _name);
	void FillPixels(DBC_CharComponent_Sections::List _type, R_Texture* _texture);

private:
	struct PixelData
	{
		PixelData& operator=(const PixelData& _other)
		{
			r = _other.r;
			g = _other.g;
			b = _other.b;
			a = _other.a;

			return *this;
		}

		uint8 r, g, b, a;
	};

	struct CharacterSkinRegion
	{
		uint32 X;
		uint32 Y;
		uint32 Width;
		uint32 Height;
	};

	PixelData*	                                              m_Pixels;
	map<DBC_CharComponent_Sections::List, CharacterSkinRegion> m_Regions;
};