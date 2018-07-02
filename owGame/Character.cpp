#include "stdafx.h"

// General
#include "Character.h"

// Additional
#include "CharSectionWrapper.h"

void Character::InitDefault()
{
	/*------------------------------------------------------------
	Char Spiritwolf, Lvl 80, Race 8, Class 7, Gender 0
	Zone 440, Map 1, Pos[-11387.166016, -4688.737305, 6.372300]
	skin 0, face 0, hairStyle 6, hairColor 1, facialStyle 4
	------------------------------------------------------------*/

	// Шаман
	/*Race = Troll;
	Class = Shaman;
	Gender = Male;
	skin = 0;
	face = 0;
	hairStyle = 6;
	hairColor = 1;
	facialStyle = 4;*/

	// Филки
	Race = Bloodelf;
	Class = Paladin;
	Gender = Female;
	skin = 2;
	face = 6;
	hairStyle = 17;
	hairColor = 9;
	facialStyle = 8;

	// TestOrc
	/*Race = Orc;
	Class = Paladin;
	Gender = Male;
	skin = 5;
	face = 6;
	hairStyle = 3;
	hairColor = 6;
	facialStyle = 5;*/

	// TEST
	/*Race = Tuskarr;
	Class = Paladin;
	Gender = Male;
	skin = 0;
	face = 0;
	hairStyle = 0;
	hairColor = 0;
	facialStyle = 0;*/

	string modelClientFileString = DBC_ChrRaces[Race]->Get_ClientFileString();
	string modelGender = Gender == Male ? "Male" : "Female";
	string fullModelName = "Character\\" + modelClientFileString + "\\" + modelGender + "\\" + modelClientFileString + modelGender + ".M2";



	M2* model = GetManager<IM2Manager>()->Add(fullModelName);

	m_Model = new CM2_Character_Instance(model);

	m_Model->getObject()->m_MeshProvider->setSpecialTexture(SM2_Texture::SM2_Texture_Type::TEX_COMPONENT_SKIN, CharSectionWrapper::getSkinTexture(this));
	m_Model->getObject()->m_MeshProvider->setSpecialTexture(SM2_Texture::SM2_Texture_Type::TEX_COMPONENT_CHAR_HAIR, CharSectionWrapper::getHairTexture(this));

	m_Model->getObject()->m_MeshProvider->setHairGeoset(CharSectionWrapper::getHairGeoset(this));

	m_Model->getObject()->m_MeshProvider->setFacial1Geoset(CharSectionWrapper::getFacial1Geoset(this));
	m_Model->getObject()->m_MeshProvider->setFacial2Geoset(CharSectionWrapper::getFacial2Geoset(this));
	m_Model->getObject()->m_MeshProvider->setFacial3Geoset(CharSectionWrapper::getFacial3Geoset(this));

	m_Model->getObject()->UpdateSpecialTextures();
}

void Character::Init(ByteBuffer& b)
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

	// read items
	for (int i = 0; i < 20; i++)
	{
		Items[i].Init(b);
	}

	// read bags
	for (int i = 0; i < 4; i++)
	{
		b.seekRelative(4);
		b.seekRelative(1);
		b.seekRelative(4);
	}
}

void Character::InitFromDisplayInfo(uint32 _id)
{
	DBC_CreatureDisplayInfoRecord* rec = DBC_CreatureDisplayInfo[_id];
	assert1(rec != nullptr);

	DBC_CreatureDisplayInfoExtraRecord* humanoidRecExtra = rec->Get_HumanoidData();

	// 1. Load model
	DBC_CreatureModelDataRecord* model = rec->Get_Model();
	assert1(rec != nullptr);

	string modelName = model->Get_ModelPath();
	M2* modelObject = GetManager<IM2Manager>()->Add(modelName);
	assert1(modelObject != nullptr);

	m_Model = new CM2_Character_Instance(modelObject);
}

void Character::Print()
{
	Log::Green("-------------------------------------------------------------");
	Log::Green("Char %s, Lvl %d, Race %d, Class %d, Gender %d", Name.c_str(), Level, Race, Class, Gender);
	Log::Green("Zone %d, Map %d, Pos[%f, %f, %f]", ZoneId, MapId, X, Y, Z);
	Log::Green("skin %d, face %d, hairStyle %d, hairColor %d, facialStyle %d", skin, face, hairStyle, hairColor, facialStyle);
	Log::Green("-------------------------------------------------------------");
}