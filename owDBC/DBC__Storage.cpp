#include "stdafx.h"

// General
#include "DBC__Storage.h"

// All
DBC_LOAD(DBC_AnimationData,				"AnimationData.dbc")
DBC_LOAD(DBC_GroundEffectDoodad,		"GroundEffectDoodad.dbc")
DBC_LOAD(DBC_GroundEffectTexture,		"GroundEffectTexture.dbc")
DBC_LOAD(DBC_LoadingScreen,				"LoadingScreens.dbc")
DBC_LOAD(DBC_Material,					"Material.dbc")
DBC_LOAD(DBC_Map,						"Map.dbc")
DBC_LOAD(DBC_TerrainType,				"TerrainType.dbc")
DBC_LOAD(DBC_WMOAreaTable,				"WMOAreaTable.dbc")
DBC_LOAD(DBC_WorldSafeLocs,				"WorldSafeLocs.dbc")

// Achivement

// Area
DBC_LOAD(DBC_AreaTable, "AreaTable.dbc")

// Character
DBC_LOAD(DBC_CharacterFacialHairStyles,	"CharacterFacialHairStyles.dbc")
DBC_LOAD(DBC_CharComponentTextureLayouts, "CharComponentTextureLayouts.dbc")
DBC_LOAD(DBC_CharComponentTextureSections, "CharComponentTextureSections.dbc")
DBC_LOAD(DBC_CharHairGeosets,	"CharHairGeosets.dbc")
DBC_LOAD(DBC_CharSections,	"CharSections.dbc")
DBC_LOAD(DBC_ChrRaces,	"ChrRaces.dbc")

// Cinematic
DBC_LOAD(DBC_CinematicCamera, "CinematicCamera.dbc")
DBC_LOAD(DBC_CinematicSequences, "CinematicSequences.dbc")

// Creature
DBC_LOAD(DBC_CreatureDisplayInfo,			"CreatureDisplayInfo.dbc")
DBC_LOAD(DBC_CreatureDisplayInfoExtra,		"CreatureDisplayInfoExtra.dbc")
DBC_LOAD(DBC_CreatureModelData,				"CreatureModelData.dbc")

// Item
DBC_LOAD(DBC_Item, "Item.dbc")
DBC_LOAD(DBC_ItemBagFamily, "ItemBagFamily.dbc")
DBC_LOAD(DBC_ItemClass, "ItemClass.dbc")
DBC_LOAD(DBC_ItemDisplayInfo, "ItemDisplayInfo.dbc")

// Light
DBC_LOAD(DBC_Light, "Light.dbc")
DBC_LOAD(DBC_LightFloatBand, "LightFloatBand.dbc")
DBC_LOAD(DBC_LightIntBand, "LightIntBand.dbc")
DBC_LOAD(DBC_LightParams, "LightParams.dbc")
DBC_LOAD(DBC_LightSkybox, "LightSkybox.dbc")

// Liquid
DBC_LOAD(DBC_LiquidMaterial, "LiquidMaterial.dbc")
DBC_LOAD(DBC_LiquidType, "LiquidType.dbc")


void OpenDBs()
{
	// All
	DBC_AnimationData.Open();
    DBC_GroundEffectDoodad.Open();
    DBC_GroundEffectTexture.Open();
    DBC_LoadingScreen.Open();
	DBC_Material.Open();
    DBC_Map.Open();
	DBC_TerrainType.Open();
    DBC_WMOAreaTable.Open();
	DBC_WorldSafeLocs.Open();
		
	// Achivement


	// Area
	DBC_AreaTable.Open();

	// Character
	DBC_CharacterFacialHairStyles.Open();
	DBC_CharComponentTextureLayouts.Open();
	DBC_CharComponentTextureSections.Open();
	DBC_CharHairGeosets.Open();
	DBC_CharSections.Open();
	DBC_ChrRaces.Open();

	// Cinematic
	DBC_CinematicCamera.Open();
	DBC_CinematicSequences.Open();

	// Creature
	DBC_CreatureDisplayInfo.Open();
	DBC_CreatureDisplayInfoExtra.Open();
	DBC_CreatureModelData.Open();

	// Item
	DBC_Item.Open();
	DBC_ItemBagFamily.Open();
	DBC_ItemClass.Open();
	DBC_ItemDisplayInfo.Open();

	// Light
	DBC_Light.Open();
	DBC_LightFloatBand.Open();
	DBC_LightIntBand.Open();
	DBC_LightParams.Open();
	DBC_LightSkybox.Open();

	// Liquid
	DBC_LiquidMaterial.Open();
	DBC_LiquidType.Open();
}
