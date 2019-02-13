#pragma once

#include "Player_Enums.h"
#include "CharacterEnums.h"
#include "ItemTemplate.h"

#include __PACK_BEGIN

struct CharacterTemplate
{
	CharacterTemplate();

	void TemplateSet(const CharacterTemplate& _o);
	void TemplateFill(CByteBuffer& b);

	void TemplateFillDefaultShaman();
	void TemplateFillDefaultPaladin();

	void TemplatePrint();

	// System
	uint64						GUID;
	std::string					Name;
	// Race
	Race::List					Race;
	Class::List					Class;
	Gender::List				Gender;
	// Style
	uint8						skin;
	uint8						face;
	uint8						hairStyle;
	uint8						hairColor;
	uint8						facialStyle;
	// Level
	uint8						Level;
	// Location
	uint32						ZoneId;
	uint32						MapId;
	float						X, Y, Z;
	// Guild
	uint32						GuildId;
	// Flags
	CharacterFlags				Flags;
	CharacterCustomizeFlags		CustumizeFlags;
	bool						IsFirstLogin;
	// Pet
	uint32						PetInfoId;
	uint32						PetLevel;
	uint32						PetFamilyId;
	// Items
	ItemTemplate				ItemsTemplates[INVENTORY_SLOT_BAG_END];
};

#include __PACK_END