#pragma once

#include "Creature.h"

#include "CharacterTemplate.h"

#include "Item_VisualData.h"

class Character : public Creature, public CharacterTemplate
{
public:
	Character();

	// Initialization
	void InitFromTemplate(const CharacterTemplate& b);
	void InitFromDisplayInfo(uint32 _id) override;

	// Mesh provider
	void setHairGeoset(uint32 _value) { setMeshEnabled(MeshIDType::SkinAndHair, _value); }
	void setFacial1Geoset(uint32 _value) { setMeshEnabled(MeshIDType::Facial1, _value); }
	void setFacial2Geoset(uint32 _value) { setMeshEnabled(MeshIDType::Facial2, _value); }
	void setFacial3Geoset(uint32 _value) { setMeshEnabled(MeshIDType::Facial3, _value); }

	// Texture components helper
	const CItem_VisualData* getItemTextureComponents(InventoryType::List _slot) const { return &m_VisualItems[_slot]; }

	// IRenderable
	void Render3D() override;

private:
	M2* CreateCharacterModel();
	void RefreshItemVisualData();

private:
	CharacterTemplate   m_Template;
	CItem_VisualData    m_VisualItems[INVENTORY_SLOT_BAG_END];
};