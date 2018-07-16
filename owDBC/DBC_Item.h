#pragma once

#include "DBC__File.h"

struct InventoryType
{
	enum List : uint8
	{
		NON_EQUIP = 0,
		HEAD = 1,
		NECK = 2,
		SHOULDERS = 3,
		BODY = 4,
		CHEST = 5,
		WAIST = 6,
		LEGS = 7,
		FEET = 8,
		WRISTS = 9,
		HANDS = 10,
		FINGER = 11,
		TRINKET = 12,
		WEAPON = 13,
		SHIELD = 14,
		RANGED = 15,
		CLOAK = 16,
		TWOHWEAPON = 17,
		BAG = 18,
		TABARD = 19,
		ROBE = 20,
		WEAPONMAINHAND = 21,
		WEAPONOFFHAND = 22,
		HOLDABLE = 23,
		AMMO = 24,
		THROWN = 25,
		RANGEDRIGHT = 26,
		QUIVER = 27,
		RELIC = 28,

		COUNT
	};
};

/*struct CharacterVisualSlots
{
	enum List : uint8
	{
		HEAD = 0,
		SHOULDERS,
		SHIRT,
		CHEST,
		BELT,
		LEGS,
		BOOTS,
		WRISTS,
		GLOVES,
		TABARD,
		CLOACK,

		// Weapons
		MAINHAND,
		OFFHAND,
		RANGED,

		COUNT
	};
};

struct
{
	InventoryType::List invType;
	CharacterVisualSlots::List visItem;
} static ItemInventoryTypeToCharacterVisualSlots[InventoryType::COUNT] =
{
	{ InventoryType::NON_EQUIP,      CharacterVisualSlots::COUNT },
	{ InventoryType::HEAD,           CharacterVisualSlots::HEAD },
	{ InventoryType::NECK,           CharacterVisualSlots::COUNT },
	{ InventoryType::SHOULDERS,      CharacterVisualSlots::SHOULDERS },
	{ InventoryType::BODY,           CharacterVisualSlots::SHIRT },
	{ InventoryType::CHEST,          CharacterVisualSlots::CHEST },
	{ InventoryType::WAIST,          CharacterVisualSlots::BELT },
	{ InventoryType::LEGS,           CharacterVisualSlots::LEGS },
	{ InventoryType::FEET,           CharacterVisualSlots::BOOTS },
	{ InventoryType::WRISTS,         CharacterVisualSlots::WRISTS },
	{ InventoryType::HANDS,          CharacterVisualSlots::GLOVES },
	{ InventoryType::FINGER,         CharacterVisualSlots::COUNT },
	{ InventoryType::TRINKET,        CharacterVisualSlots::COUNT },
	{ InventoryType::WEAPON,         CharacterVisualSlots::MAINHAND },
	{ InventoryType::SHIELD,         CharacterVisualSlots::OFFHAND },
	{ InventoryType::RANGED,         CharacterVisualSlots::RANGED },
	{ InventoryType::CLOAK,          CharacterVisualSlots::CLOACK },
	{ InventoryType::TWOHWEAPON,     CharacterVisualSlots::MAINHAND },
	{ InventoryType::BAG,            CharacterVisualSlots::COUNT },
	{ InventoryType::TABARD,         CharacterVisualSlots::TABARD },
	{ InventoryType::ROBE,           CharacterVisualSlots::COUNT }, // ???
	{ InventoryType::WEAPONMAINHAND, CharacterVisualSlots::MAINHAND },
	{ InventoryType::WEAPONOFFHAND,  CharacterVisualSlots::OFFHAND },
	{ InventoryType::HOLDABLE,       CharacterVisualSlots::COUNT },
	{ InventoryType::AMMO,           CharacterVisualSlots::COUNT },
	{ InventoryType::THROWN,         CharacterVisualSlots::RANGED },
	{ InventoryType::RANGEDRIGHT,    CharacterVisualSlots::RANGED }, // ???
	{ InventoryType::QUIVER,         CharacterVisualSlots::COUNT },
	{ InventoryType::RELIC,          CharacterVisualSlots::COUNT }
};*/

DBC_DEF_BEGIN(DBC_Item)

__DBC_REF_ID(DBC_ItemClass,        ItemClassID,            2);
__DBC_TVALUE(uint32,               ItemSubclassID,         3);
__DBC_TVALUE(int32,                SoundSubclassOverride,  4);
__DBC_REF_ID(DBC_Material,         MaterialID,             5);
__DBC_REF_ID(DBC_ItemDisplayInfo,  DisplayInfoID,          6);
__DBC_TVALUE(InventoryType::List,  InventorySlot,          7);
__DBC_TVALUE(uint32,               SheathID,               8);

DBC_DEF_END
