#pragma once

#include "MeshIDEnums.h"
#include "CharacterEnums.h"

#include "ItemTemplate.h"
#include "Item_M2Instance.h"

// FORWARD BEGIN
class Character;
class Creature_M2Instance;
// FORWARD END

struct ObjectComponent
{
	CItem_M2Instance*           model;
	SharedTexturePtr            texture;
	const CM2_Part_Attachment*	attach;
};

struct GeosetComponent
{
	MeshIDType::List  mesh;
	uint32            value;
	uint32 getMeshID() const { return 1 + value; }
};

class CItem_VisualData : public ItemTemplate
{
public:
	CItem_VisualData();
	CItem_VisualData(uint32	_displayId, InventoryType::List _inventoryType, uint32 _enchantAuraID);

	void Load(Character* _character);

	const vector<ObjectComponent>&  getObjectComponents() const { return m_ObjectComponents; }
	const vector<GeosetComponent>&  getGeosetComponents() const { return m_GeosetComponents; }
	R_Texture*                      getTextureComponent(DBC_CharComponent_Sections::List _type) const { return m_TextureComponents[_type]; }

	void Render3D();

private:
	void InitObjectComponents(Character* _parent);
	void InitGeosetComponents();
	void InitTextureComponents(Gender::List _gender);

	M2*        LoadObjectModel   (InventoryType::List _objectType, string _modelName);
	R_Texture* LoadObjectTexture (InventoryType::List _objectType, string _textureName);
	R_Texture* LoadSkinTexture   (DBC_CharComponent_Sections::List _type, string _textureName);
	

	// Helpers
	string getTextureComponentName(DBC_CharComponent_Sections::List _type, string _textureName, Gender::List _gender);
	char getGenderLetter(Gender::List _gender);

private:
	vector<ObjectComponent>  m_ObjectComponents;
	vector<GeosetComponent>  m_GeosetComponents;
	SharedTexturePtr         m_TextureComponents[DBC_CharComponent_Sections::ITEMS_COUNT];
	
};