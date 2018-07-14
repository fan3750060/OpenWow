#include "stdafx.h"

// Include
#include "Character.h"

// General
#include "CharSectionWrapper.h"

/*
			0 -	Base Skin		1 - Face			2 - Facial Hair		3 - Hair			4 - Underwear
Type		-					FaceType			FacialHairType		HairStyle			-
Texture1	SkinTexture			FaceLowerTexture	FacialLowerTexture	HairTexture			PelvisTexture
Texture2	ExtraSkinTexture	FaceUpperTexture	FacialUpperTexture	ScalpLowerTexture	TorsoTexture
Texture3	-					-					-					ScalpUpperTexture	-
*/

string CharSectionWrapper::getSkinTexture(Character* _character)
{
	for (auto& i : DBC_CharSections)
	{
		if (i.Get_GeneralType() == DBC_CharSections_GeneralType::Skin &&
			i.Get_Race() == _character->Race &&
			i.Get_Gender() == _character->Gender &&
			i.Get_VariationOrColor() == _character->skin)
		{
			return i.Get_Texture1();
		}
	}

	fail1();
	return "";
}

//------------------------------------------------------------
//-- Face
//------------------------------------------------------------

string CharSectionWrapper::getFaceLowerTexture(Character* _character)
{
	for (auto& i : DBC_CharSections)
	{
		if (
			i.Get_GeneralType() == DBC_CharSections_GeneralType::Face &&
			i.Get_Race() == _character->Race &&
			i.Get_Gender() == _character->Gender &&
			i.Get_VariationOrColor() == _character->face
			)
		{
			return i.Get_Texture1();
		}
	}

	fail1();
	return "";
}

string CharSectionWrapper::getFaceUpperTexture(Character* _character)
{
	for (auto& i : DBC_CharSections)
	{
		if (
			i.Get_GeneralType() == DBC_CharSections_GeneralType::Face &&
			i.Get_Race() == _character->Race &&
			i.Get_Gender() == _character->Gender &&
			i.Get_VariationOrColor() == _character->face
			)
		{
			return i.Get_Texture2();
		}
	}

	fail1();
	return "";
}

//------------------------------------------------------------
//-- FañialHair
//------------------------------------------------------------

string CharSectionWrapper::getFacialHairLowerTexture(Character* _character)
{
	for (auto& i : DBC_CharSections)
	{
		if (
			i.Get_GeneralType() == DBC_CharSections_GeneralType::FacialHair &&
			i.Get_Race() == _character->Race &&
			i.Get_Gender() == _character->Gender &&
			i.Get_VariationOrColor() == _character->facialStyle
			)
		{
			return i.Get_Texture1();
		}
	}

	fail1();
	return "";
}

string CharSectionWrapper::getFacialHairUpperTexture(Character* _character)
{
	for (auto& i : DBC_CharSections)
	{
		if (
			i.Get_GeneralType() == DBC_CharSections_GeneralType::FacialHair &&
			i.Get_Race() == _character->Race &&
			i.Get_Gender() == _character->Gender &&
			i.Get_VariationOrColor() == _character->facialStyle
			)
		{
			return i.Get_Texture2();
		}
	}

	fail1();
	return "";
}

uint32 CharSectionWrapper::getFacial1Geoset(Character * _character)
{
	for (auto& it = DBC_CharacterFacialHairStyles.begin(); it != DBC_CharacterFacialHairStyles.end(); ++it)
	{
		if (
			it->Get_Race() == _character->Race &&
			it->Get_Gender() == _character->Gender &&
			it->Get_Variation() == _character->facialStyle
			)
		{
			return it->Get_Group_01xx();
		}
	}

	fail1();
	return UINT32_MAX;
}

uint32 CharSectionWrapper::getFacial2Geoset(Character * _character)
{
	for (auto& i : DBC_CharacterFacialHairStyles)
	{
		if (
			i.Get_Race() == _character->Race &&
			i.Get_Gender() == _character->Gender &&
			i.Get_Variation() == _character->facialStyle
			)
		{
			return i.Get_Group_02xx();
		}
	}

	fail1();
	return UINT32_MAX;
}

uint32 CharSectionWrapper::getFacial3Geoset(Character * _character)
{
	for (auto& i : DBC_CharacterFacialHairStyles)
	{
		if (
			i.Get_Race() == _character->Race &&
			i.Get_Gender() == _character->Gender &&
			i.Get_Variation() == _character->facialStyle
			)
		{
			return i.Get_Group_03xx();
		}
	}

	fail1();
	return UINT32_MAX;
}

//------------------------------------------------------------
//-- Hair
//------------------------------------------------------------

string CharSectionWrapper::getHairTexture(Character* _character)
{
	for (auto& i : DBC_CharSections)
	{
		if (
			i.Get_GeneralType() == DBC_CharSections_GeneralType::Hair &&
			i.Get_Race() == _character->Race &&
			i.Get_Gender() == _character->Gender &&
			i.Get_Type() == _character->hairStyle &&
			i.Get_VariationOrColor() == _character->hairColor
			)
		{
			return i.Get_Texture1();
		}
	}

	fail1();
	return "";
}

string CharSectionWrapper::getHairScalpLowerTexture(Character* _character)
{
	for (auto& i : DBC_CharSections)
	{
		if (
			i.Get_GeneralType() == DBC_CharSections_GeneralType::Hair &&
			i.Get_Race() == _character->Race &&
			i.Get_Gender() == _character->Gender &&
			i.Get_VariationOrColor() == _character->hairStyle
			)
		{
			return i.Get_Texture2();
		}
	}

	fail1();
	return "";
}

string CharSectionWrapper::getHairScalpUpperTexture(Character* _character)
{
	for (auto& i : DBC_CharSections)
	{
		if (
			i.Get_GeneralType() == DBC_CharSections_GeneralType::Hair &&
			i.Get_Race() == _character->Race &&
			i.Get_Gender() == _character->Gender &&
			i.Get_VariationOrColor() == _character->hairStyle
			)
		{
			return i.Get_Texture3();
		}
	}

	fail1();
	return "";
}

uint32 CharSectionWrapper::getHairGeoset(Character* _character)
{
	/*DBC_CharSectionsRecord* charSections = nullptr;
	for (auto i = DBC_CharSections.Records().begin(); i != DBC_CharSections.Records().end(); ++i)
	{
		if (
			i->second->Get_GeneralType() == DBC_CharSections_GeneralType::Hair &&
			i->second->Get_Race() == _character->Race &&
			i->second->Get_Gender() == _character->Gender &&
			i->second->Get_VariationOrColor() == _character->hairStyle
			)
		{
			charSections = i->second;
		}
	}
	assert1(charSections != nullptr);*/

	const DBC_CharHairGeosetsRecord* charHairRecord = nullptr;
	for (auto& i : DBC_CharHairGeosets)
	{
		if (
			i.Get_Race() == _character->Race &&
			i.Get_Gender() == _character->Gender &&
			i.Get_HairType() == _character->hairStyle
			)
		{
			charHairRecord = &i;
		}
	}
	assert1(charHairRecord != nullptr);

	return charHairRecord->Get_Geoset();
}
