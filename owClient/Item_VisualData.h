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
	CItem_VisualData(Character* _owner);

	void Load();

	const vector<ObjectComponent>&  getObjectComponents() const { return m_ObjectComponents; }
	const vector<GeosetComponent>&  getGeosetComponents() const { return m_GeosetComponents; }
	SharedTexturePtr                getTextureComponent(DBC_CharComponent_Sections::List _type) const { return m_TextureComponents[_type]; }

	void Render3D();

private:
	void InitObjectComponents();
	void InitGeosetComponents();
	void InitTextureComponents();

	SmartM2Ptr        LoadObjectModel   (InventoryType::List _objectType, string _modelName);
	SharedTexturePtr LoadObjectTexture (InventoryType::List _objectType, string _textureName);
	SharedTexturePtr LoadSkinTexture   (DBC_CharComponent_Sections::List _type, string _textureName);
	
	// Helpers
	string getTextureComponentName(DBC_CharComponent_Sections::List _type, string _textureName, Gender::List _gender);
	char getGenderLetter(Gender::List _gender);

private:
	vector<ObjectComponent>  m_ObjectComponents;
	vector<GeosetComponent>  m_GeosetComponents;
	SharedTexturePtr         m_TextureComponents[DBC_CharComponent_Sections::ITEMS_COUNT];
	
private: // PARENT
	Character* m_ParentCharacter;
};