#include "stdafx.h"

// General
#include "Creature.h"

// Additional
#include "CreatureMeshIDProvider.h"


/*
m_FileName="Creature\\Alexstrasza\\Alexstrasza.m2"

*/

Creature::Creature(vec3 _position) :
	CM2_Base_Instance(nullptr),
	m_CreaturePosition(_position)
{
	setDebugColor(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	setSelectable();
}

void Creature::InitFromDisplayInfo(uint32 _id)
{
	DBC_CreatureDisplayInfoRecord* rec = DBC_CreatureDisplayInfo[_id];
	assert1(rec != nullptr);

	DBC_CreatureDisplayInfoExtraRecord* humanoidRecExtra = rec->Get_HumanoidData();

	// 1. Load model
	{
		DBC_CreatureModelDataRecord* modelRec = rec->Get_Model();
		assert1(modelRec != nullptr);

		string modelName = modelRec->Get_ModelPath();
		M2* m2 = GetManager<IM2Manager>()->Add(modelName);
		assert1(m2 != nullptr);
		setObject(m2);
		m_CreatureScale = modelRec->Get_Scale();

		RecalculateMatrix();
		InitLocal();
	}

	// 2. Create MeshProvider
	{
		CCreatureMeshIDProvider* provider = new CCreatureMeshIDProvider();

		if (strlen(rec->Get_Texture1()) != 0)
			provider->setSpecialTexture(SM2_Texture::SM2_Texture_Type::TEX_COMPONENT_MONSTER_1, getObject()->getFilePath() + rec->Get_Texture1() + ".blp");

		if (strlen(rec->Get_Texture2()) != 0)
			provider->setSpecialTexture(SM2_Texture::SM2_Texture_Type::TEX_COMPONENT_MONSTER_2, getObject()->getFilePath() + rec->Get_Texture2() + ".blp");

		if (strlen(rec->Get_Texture3()) != 0)
			provider->setSpecialTexture(SM2_Texture::SM2_Texture_Type::TEX_COMPONENT_MONSTER_3, getObject()->getFilePath() + rec->Get_Texture3() + ".blp");

		setMeshProvider(provider);
	}
}

void Creature::RecalculateMatrix()
{
	m_Translate = m_CreaturePosition;
	m_Scale = vec3(m_CreatureScale);

	CalculateMatrix();

	m_Bounds = m_Object->getBounds();
	m_Bounds.transform(getAbsTrans());
}
