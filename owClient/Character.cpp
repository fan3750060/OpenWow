#include "stdafx.h"

// General
#include "Character.h"

// Additional
#include "Character_SectionWrapper.h"
#include "Character_SkinTextureBaker.h"

Character::Character() :
	Creature()
{
	setMeshEnabled(MeshIDType::Ears, EarsStyles::Enabled);
	setMeshEnabled(MeshIDType::Eyeglows, EyeglowsStyles::Racial);
}

void Character::InitFromTemplate(const CharacterTemplate& b)
{
	// 1. Template
	{
		TemplateSet(b);
	}

	// 2. Load model
	{
		CreateCharacterModel();
	}

	// 3. Items visual data
	RefreshItemVisualData();

	// 4. Creature textures
	{
		Character_SkinTextureBaker baker;

		// Default textures
		setSpecialTexture(SM2_Texture::Type::SKIN, baker.createTexture(this));
		setSpecialTexture(SM2_Texture::Type::SKIN_EXTRA, Character_SectionWrapper::getSkinExtraTexture(this));
		setSpecialTexture(SM2_Texture::Type::CHAR_HAIR, Character_SectionWrapper::getHairTexture(this));

		// Cloak
		CItem_VisualData& item = m_VisualItems[EQUIPMENT_SLOT_BACK];
		if (item.InventoryType != InventoryType::NON_EQUIP)
		{
			assert1(item.getObjectComponents().size() == 1);
			R_Texture* cloackTexttre = item.getObjectComponents()[0].texture;
			setSpecialTexture(SM2_Texture::Type::OBJECT_SKIN, cloackTexttre);
		}

		// Geosets
		setHairGeoset(Character_SectionWrapper::getHairGeoset(this));
		setFacial1Geoset(Character_SectionWrapper::getFacial1Geoset(this));
		setFacial2Geoset(Character_SectionWrapper::getFacial2Geoset(this));
		setFacial3Geoset(Character_SectionWrapper::getFacial3Geoset(this));
	}
}

void Character::InitFromDisplayInfo(uint32 _id)
{
	DBC_CreatureDisplayInfoRecord* rec = DBC_CreatureDisplayInfo[_id];
	assert1(rec != nullptr);

	DBC_CreatureDisplayInfoExtraRecord* humanoidRecExtra = rec->Get_HumanoidData();
	assert1(humanoidRecExtra != nullptr);

	// 1. Template
	{
		Race = (Race::List)humanoidRecExtra->Get_Race()->Get_ID();
		Gender = (Gender::List)humanoidRecExtra->Get_Gender();
		skin = humanoidRecExtra->Get_SkinColor();
		face = humanoidRecExtra->Get_FaceType();
		hairStyle = humanoidRecExtra->Get_HairType();
		hairColor = humanoidRecExtra->Get_HairStyleOrColor();
		facialStyle = humanoidRecExtra->Get_BeardStyle();

		ItemsTemplates[EQUIPMENT_SLOT_HEAD] = ItemTemplate(humanoidRecExtra->Get_Helm(), InventoryType::HEAD, 0);
		ItemsTemplates[EQUIPMENT_SLOT_SHOULDERS] = ItemTemplate(humanoidRecExtra->Get_Shoulder(), InventoryType::SHOULDERS, 0);
		ItemsTemplates[EQUIPMENT_SLOT_BODY] = ItemTemplate(humanoidRecExtra->Get_Shirt(), InventoryType::BODY, 0);
		ItemsTemplates[EQUIPMENT_SLOT_CHEST] = ItemTemplate(humanoidRecExtra->Get_Chest(), InventoryType::CHEST, 0);
		ItemsTemplates[EQUIPMENT_SLOT_WAIST] = ItemTemplate(humanoidRecExtra->Get_Belt(), InventoryType::WAIST, 0);
		ItemsTemplates[EQUIPMENT_SLOT_LEGS] = ItemTemplate(humanoidRecExtra->Get_Legs(), InventoryType::LEGS, 0);
		ItemsTemplates[EQUIPMENT_SLOT_FEET] = ItemTemplate(humanoidRecExtra->Get_Boots(), InventoryType::FEET, 0);
		ItemsTemplates[EQUIPMENT_SLOT_WRISTS] = ItemTemplate(humanoidRecExtra->Get_Wrist(), InventoryType::WRISTS, 0);
		ItemsTemplates[EQUIPMENT_SLOT_HANDS] = ItemTemplate(humanoidRecExtra->Get_Gloves(), InventoryType::HANDS, 0);
		ItemsTemplates[EQUIPMENT_SLOT_TABARD] = ItemTemplate(humanoidRecExtra->Get_Tabard(), InventoryType::TABARD, 0);
		ItemsTemplates[EQUIPMENT_SLOT_BACK] = ItemTemplate(humanoidRecExtra->Get_Cape(), InventoryType::CLOAK, 0);
	}

	// 2. Load model
	{
		// We ???always??? can load model from (CreatureDisplayInfo->Model)
		CreateCreatureModel(rec);
	}

	// 3. Items
	RefreshItemVisualData();

	// 2. Character data
	Race = (Race::List)humanoidRecExtra->Get_Race()->Get_ID();
	Gender = (Gender::List)humanoidRecExtra->Get_Gender();
	skin = humanoidRecExtra->Get_SkinColor();
	face = humanoidRecExtra->Get_FaceType();
	hairStyle = humanoidRecExtra->Get_HairType();
	hairColor = humanoidRecExtra->Get_HairStyleOrColor();
	facialStyle = humanoidRecExtra->Get_BeardStyle();

	// 3. Creature textures
	{
		// Default textures
		string bakedTextureName = humanoidRecExtra->Get_Texture();
		R_Texture* bakedSkinTexture = nullptr;
		if (bakedTextureName.empty() || !(CMPQFile::IsFileExists("Textures\\BakedNpcTextures\\" + bakedTextureName)))
		{
			Character_SkinTextureBaker baker;
			bakedSkinTexture = baker.createTexture(this);

			Log::Error("Character[%d]: Missing baked texture for humanoid[%d]. Create own.", rec->Get_ID(), humanoidRecExtra->Get_ID());
		}
		else
		{
			bakedSkinTexture = GetManager<ITexturesManager>()->Add("Textures\\BakedNpcTextures\\" + bakedTextureName);
		}

		setSpecialTexture(SM2_Texture::Type::SKIN, bakedSkinTexture);
		setSpecialTexture(SM2_Texture::Type::SKIN_EXTRA, Character_SectionWrapper::getSkinExtraTexture(this));
		setSpecialTexture(SM2_Texture::Type::CHAR_HAIR, Character_SectionWrapper::getHairTexture(this));

		// Cloak
		CItem_VisualData& item = m_VisualItems[EQUIPMENT_SLOT_BACK];
		if (item.InventoryType != InventoryType::NON_EQUIP)
		{
			assert1(item.getObjectComponents().size() == 1);
			R_Texture* cloackTexttre = item.getObjectComponents()[0].texture;
			setSpecialTexture(SM2_Texture::Type::OBJECT_SKIN, cloackTexttre);
		}

		// Geosets
		setHairGeoset   (Character_SectionWrapper::getHairGeoset   (this));
		setFacial1Geoset(Character_SectionWrapper::getFacial1Geoset(this));
		setFacial2Geoset(Character_SectionWrapper::getFacial2Geoset(this));
		setFacial3Geoset(Character_SectionWrapper::getFacial3Geoset(this));
	}
}

//--



//--

void Character::Render3D()
{
	CM2_Base_Instance::Render3D();

	for (uint32 slot = 0; slot < INVENTORY_SLOT_BAG_END; slot++)
	{
		if (slot == EQUIPMENT_SLOT_HEAD && (Flags & CHARACTER_FLAG_HIDE_HELM))
		{
			continue;
		}

		m_VisualItems[slot].Render3D();
	}
}

//--

M2* Character::CreateCharacterModel()
{
	string modelClientFileString = DBC_ChrRaces[Race]->Get_ClientFileString();
	string modelGender = (Gender == Gender::Male) ? "Male" : "Female";
	string fullModelName = "Character\\" + modelClientFileString + "\\" + modelGender + "\\" + modelClientFileString + modelGender + ".M2";

	M2* model = GetManager<IM2Manager>()->Add(fullModelName);
	assert1(model != nullptr);

	setM2(model);

	CalculateMatrix();
	InitLocal();

	return model;
}

void Character::RefreshItemVisualData()
{
	// 3. Items visual data
	for (uint32 i = 0; i < INVENTORY_SLOT_BAG_END; i++)
	{
		m_VisualItems[i].TemplateSet(ItemsTemplates[i]);
		m_VisualItems[i].Load(this);

		if (i == EQUIPMENT_SLOT_HEAD)
		{
			if ((Flags & CHARACTER_FLAG_HIDE_HELM))
			{
				setMeshEnabled(MeshIDType::Ears, EarsStyles::Enabled);
				continue;
			}
			else
			{
				setMeshEnabled(MeshIDType::Ears, EarsStyles::None);
			}
		}
		else if (i == EQUIPMENT_SLOT_BACK)
		{
			if ((Flags & CHARACTER_FLAG_HIDE_CLOAK))
			{
				setMeshEnabled(MeshIDType::Cloak, 1);
				continue;
			}
		}

		for (auto& geoset : m_VisualItems[i].getGeosetComponents())
		{
			setMeshEnabled(geoset.mesh, geoset.getMeshID());
		}
	}
}

