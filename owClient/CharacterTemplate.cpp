#include "stdafx.h"

// General
#include "CharacterTemplate.h"

CharacterTemplate::CharacterTemplate() :
	// System
	GUID(0),
	Name("Default name"),
	// Race
	Race(Race::Human),
	Class(Class::Warrior),
	Gender(Gender::Male),
	// Style
	skin(0),
	face(0),
	hairStyle(0),
	hairColor(0),
	facialStyle(0),
	// Level
	Level(255),
	// Location
	ZoneId(0),
	MapId(0),
	X(0.0f),
	Y(0.0f),
	Z(0.0f),
	// Guild
	GuildId(0),
	// Flags
	Flags(CharacterFlags::CHARACTER_FLAG_NONE),
	CustumizeFlags(CharacterCustomizeFlags::CHAR_CUSTOMIZE_FLAG_NONE),
	IsFirstLogin(true),
	// Pet
	PetInfoId(0),
	PetLevel(0),
	PetFamilyId(0)
{}

void CharacterTemplate::TemplateSet(const CharacterTemplate & _o)
{
	// System
	GUID = _o.GUID;
	Name = _o.Name;
	// Race
	Race = _o.Race;
	Class = _o.Class;
	Gender = _o.Gender;
	// Style
	skin = _o.skin;
	face = _o.face;
	hairStyle = _o.hairStyle;
	hairColor = _o.hairColor;
	facialStyle = _o.facialStyle;
	// Level
	Level = _o.Level;
	// Location
	ZoneId = _o.ZoneId;
	MapId = _o.MapId;
	X = _o.X;
	Y = _o.Y;
	Z = _o.Z;
	// Guild
	GuildId = _o.GuildId;
	// Flags
	Flags = _o.Flags;
	CustumizeFlags = _o.CustumizeFlags;
	IsFirstLogin = _o.IsFirstLogin;
	// Pet
	PetInfoId = _o.PetInfoId;
	PetLevel = _o.PetLevel;
	PetFamilyId = _o.PetFamilyId;

	// ItemsTemplates
	for (int i = 0; i < INVENTORY_SLOT_BAG_END; i++)
	{
		ItemsTemplates[i] = _o.ItemsTemplates[i];
	}
}

void CharacterTemplate::TemplateFill(ByteBuffer& b)
{
	uint32 beginSize = b.getPos();

	b.readBytes(&GUID, 8);
	b.readString(&Name);
	b.readBytes(&Race, 1);
	b.readBytes(&Class, 1);
	b.readBytes(&Gender, 1);

	b.readBytes(&skin, 1);
	b.readBytes(&face, 1);
	b.readBytes(&hairStyle, 1);
	b.readBytes(&hairColor, 1);
	b.readBytes(&facialStyle, 1);

	b.readBytes(&Level, 1);
	b.readBytes(&ZoneId, 4);
	b.readBytes(&MapId, 4);

	b.readBytes(&X, 4);
	b.readBytes(&Y, 4);
	b.readBytes(&Z, 4);

	b.readBytes(&GuildId, 4);
	b.readBytes(&Flags, 4);
	b.readBytes(&CustumizeFlags, 4);
	b.readBytes(&IsFirstLogin, 1);

	b.readBytes(&PetInfoId, 4);
	b.readBytes(&PetLevel, 4);
	b.readBytes(&PetFamilyId, 4);

	for (int i = 0; i < INVENTORY_SLOT_BAG_END; i++)
	{
		ItemsTemplates[i].TemplateFill(b);
	}
}

void CharacterTemplate::TemplateFillDefaultShaman()
{
	Race = Race::Troll;
	Class = Class::Shaman;
	Gender = Gender::Male;
	skin = 0;
	face = 0;
	hairStyle = 6;
	hairColor = 1;
	facialStyle = 4;
	
	Flags = CHARACTER_FLAG_HIDE_HELM;

	ItemsTemplates[0] = ItemTemplate(47037, (InventoryType::List) 1, 0);
	ItemsTemplates[1] = ItemTemplate(64219, (InventoryType::List) 2, 0);
	ItemsTemplates[2] = ItemTemplate(46990, (InventoryType::List) 3, 0);
	ItemsTemplates[3] = ItemTemplate((uint32)0, (InventoryType::List) 0, 0);
	ItemsTemplates[4] = ItemTemplate(46995, (InventoryType::List) 5, 0);
	ItemsTemplates[5] = ItemTemplate(60345, (InventoryType::List) 6, 0);
	ItemsTemplates[6] = ItemTemplate(65172, (InventoryType::List) 7, 0);
	ItemsTemplates[7] = ItemTemplate(52163, (InventoryType::List) 8, 0);
	ItemsTemplates[8] = ItemTemplate(65185, (InventoryType::List) 9, 0);
	ItemsTemplates[9] = ItemTemplate(47034, (InventoryType::List) 10, 0);
	ItemsTemplates[10] = ItemTemplate(39127, (InventoryType::List) 11, 0);
	ItemsTemplates[11] = ItemTemplate(39127, (InventoryType::List) 11, 0);
	ItemsTemplates[12] = ItemTemplate(64247, (InventoryType::List) 12, 0);
	ItemsTemplates[13] = ItemTemplate(68106, (InventoryType::List) 12, 0);
	ItemsTemplates[14] = ItemTemplate(46455, (InventoryType::List) 16, 0);
	ItemsTemplates[15] = ItemTemplate(64676, (InventoryType::List) 21, 160);
	ItemsTemplates[16] = ItemTemplate(44710, (InventoryType::List) 14, 0);
	ItemsTemplates[17] = ItemTemplate(64629, (InventoryType::List) 28, 0);
	ItemsTemplates[18] = ItemTemplate((uint32)0, (InventoryType::List) 0, 0);

	ItemsTemplates[19] = ItemTemplate(34780, (InventoryType::List) 18, 0); // Bags
	ItemsTemplates[20] = ItemTemplate(34780, (InventoryType::List) 18, 0);
	ItemsTemplates[21] = ItemTemplate(34780, (InventoryType::List) 18, 0);
	ItemsTemplates[22] = ItemTemplate(39459, (InventoryType::List) 18, 0);
}

void CharacterTemplate::TemplateFillDefaultPaladin()
{
	Race = Race::Bloodelf;
	Class = Class::Paladin;
	Gender = Gender::Female;
	skin = 2;
	face = 6;
	hairStyle = 17;
	hairColor = 9;
	facialStyle = 8;

	Flags = CHARACTER_FLAG_HIDE_HELM;

	ItemsTemplates[0] = ItemTemplate(64587, (InventoryType::List) InventoryType::HEAD, 0);
	ItemsTemplates[1] = ItemTemplate(64190, (InventoryType::List) 2, 0);
	ItemsTemplates[2] = ItemTemplate(64998, InventoryType::SHOULDERS, 0);
	ItemsTemplates[3] = ItemTemplate((uint32)0, InventoryType::NON_EQUIP, 0);
	ItemsTemplates[4] = ItemTemplate(64658, InventoryType::CHEST, 0);
	ItemsTemplates[5] = ItemTemplate(64786, InventoryType::WAIST, 0);
	ItemsTemplates[6] = ItemTemplate(64588, InventoryType::LEGS, 0);
	ItemsTemplates[7] = ItemTemplate(64414, InventoryType::FEET, 0);
	ItemsTemplates[8] = ItemTemplate(64833, InventoryType::WRISTS, 0);
	ItemsTemplates[9] = ItemTemplate(64593, InventoryType::HANDS, 0);
	ItemsTemplates[10] = ItemTemplate(64230, (InventoryType::List) 11, 0);
	ItemsTemplates[11] = ItemTemplate(64169, (InventoryType::List) 11, 0);
	ItemsTemplates[12] = ItemTemplate(64254, (InventoryType::List) 12, 0);
	ItemsTemplates[13] = ItemTemplate(29947, (InventoryType::List) 12, 0);
	ItemsTemplates[14] = ItemTemplate(61403, InventoryType::CLOAK, 0);
	ItemsTemplates[15] = ItemTemplate(/*63876*/65153, (InventoryType::List) 17, /*178*/165);
	ItemsTemplates[16] = ItemTemplate((uint32)0, InventoryType::NON_EQUIP, 0);
	ItemsTemplates[17] = ItemTemplate(34960, (InventoryType::List) 28, 0);
	ItemsTemplates[18] = ItemTemplate((uint32)0, InventoryType::NON_EQUIP, 0);

	ItemsTemplates[19] = ItemTemplate(34780, (InventoryType::List) 18, 0);
	ItemsTemplates[20] = ItemTemplate(34780, (InventoryType::List) 18, 0);
	ItemsTemplates[21] = ItemTemplate(34780, (InventoryType::List) 18, 0);
	ItemsTemplates[22] = ItemTemplate(39459, (InventoryType::List) 18, 0);
}

void CharacterTemplate::TemplatePrint()
{
	Log::Info("Char %s, -------------------------------------------------------------", Name.c_str());
	Log::Info("Lvl %d, Race %d, Class %d, Gender %d", Level, Race, Class, Gender);
	Log::Info("Zone %d, Map %d, Pos[%f, %f, %f]", ZoneId, MapId, X, Y, Z);
	Log::Info("skin %d, face %d, hairStyle %d, hairColor %d, facialStyle %d", skin, face, hairStyle, hairColor, facialStyle);
}
