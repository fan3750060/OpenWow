#pragma once

#include "CharacterEnums.h"
#include "M2_Character_Instance.h"

struct Item
{
	void Init(ByteBuffer& b)
	{
		b.readBytes(&DisplayId, 4);
		b.readBytes(&InventoryType, 1);
		b.seekRelative(4);
	}

public:
	uint32 DisplayId;
	uint8 InventoryType;
};


class Character
{
public:
	void InitDefault();
	void Init(ByteBuffer& b);

	void Print();

public:
	// System
	uint64						GUID;
	string						Name;
	// Race
	Race						Race;
	Class						Class;
	Gender						Gender;
	// Items
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
	Item						Items[20];

	// Model
public:
	CM2_Character_Instance* m_Model;
	R_Texture*				m_SkinTexture;
};