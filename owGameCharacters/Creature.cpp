#include "stdafx.h"

// General
#include "Creature.h"

/*
m_FileName="Creature\\Alexstrasza\\Alexstrasza.m2"

*/

Creature::Creature() :
	Creature_M2Instance()
{
	for (uint32 i = 0; i < MeshIDType::Count; i++)
	{
		m_MeshID[i] = 1;
	}
}

void Creature::InitFromDisplayInfo(uint32 _id)
{
	std::shared_ptr<const DBC_CreatureDisplayInfoRecord> rec = DBC_CreatureDisplayInfo[_id];
	assert1(rec != nullptr);

	std::shared_ptr<const DBC_CreatureDisplayInfoExtraRecord> humanoidRecExtra = rec->Get_HumanoidData();
	assert1(humanoidRecExtra == nullptr);

	// 1. Load model
	{
		CreateCreatureModel(rec);
	}

	// 2. Creature textures
	{

		if (strlen(rec->Get_Texture1()) != 0)
		{
			setSpecialTexture(SM2_Texture::Type::MONSTER_1, getM2()->getFilePath() + rec->Get_Texture1() + ".blp");
		}

		if (strlen(rec->Get_Texture2()) != 0)
		{
			setSpecialTexture(SM2_Texture::Type::MONSTER_2, getM2()->getFilePath() + rec->Get_Texture2() + ".blp");
		}

		if (strlen(rec->Get_Texture3()) != 0)
		{
			setSpecialTexture(SM2_Texture::Type::MONSTER_3, getM2()->getFilePath() + rec->Get_Texture3() + ".blp");
		}
	}
}

void Creature::setMeshEnabled(MeshIDType::List _type, uint32 _value)
{
	assert1(_type < MeshIDType::Count);
	if (_value == UINT32_MAX)
	{
		return;
	}

	m_MeshID[_type] = _value;
}

bool Creature::isMeshEnabled(uint32 _index) const
{
	uint32 div100 = _index / 100;
	uint32 mod100 = _index % 100;

	//assert1(div100 < MeshIDType::Count);
	assert1(div100 != 6);
	assert1(div100 != 14);
	assert1(div100 != 16);

	for (uint32 i = 0; i < MeshIDType::Count; i++)
	{
		// Special case for skin
		if (div100 == 0 && mod100 == 0)
		{
			return true;
		}

		// Others
		if (div100 == i)
		{
			if (m_MeshID[i] == mod100)
			{
				return true;
			}
		}
	}

	return false;
}

void Creature::CreateCreatureModel(std::shared_ptr<const DBC_CreatureDisplayInfoRecord> _record)
{
	std::shared_ptr<const DBC_CreatureModelDataRecord> modelRec = _record->Get_Model();
	assert1(modelRec != nullptr);

	std::string modelName = modelRec->Get_ModelPath();
	std::shared_ptr<M2> m2 = GetManager<IM2Manager>()->Add(modelName);
	assert1(m2 != nullptr);

	setM2(m2);

	setAlpha(static_cast<float>(_record->Get_Opacity()) / 255.0f);
	//SetScale(_record.Get_Scale());
}

