#include "stdafx.h"

// General
#include "DBC__Storage.h"

// Tables
DBC_LOAD(DBC_AnimationData,				"AnimationData.dbc")
DBC_LOAD(DBC_AreaTable,					"AreaTable.dbc")
DBC_LOAD(DBC_CinematicCamera,			"CinematicCamera.dbc")
DBC_LOAD(DBC_CinematicSequences,		"CinematicSequences.dbc")
DBC_LOAD(DBC_GroundEffectDoodad,		"GroundEffectDoodad.dbc")
DBC_LOAD(DBC_GroundEffectTexture,		"GroundEffectTexture.dbc")
DBC_LOAD(DBC_Light,						"Light.dbc")
DBC_LOAD(DBC_LightFloatBand,			"LightFloatBand.dbc")
DBC_LOAD(DBC_LightIntBand,				"LightIntBand.dbc")
DBC_LOAD(DBC_LightParams,				"LightParams.dbc")
DBC_LOAD(DBC_LightSkybox,				"LightSkybox.dbc")
DBC_LOAD(DBC_LiquidMaterial,			"LiquidMaterial.dbc")
DBC_LOAD(DBC_LiquidType,				"LiquidType.dbc")
DBC_LOAD(DBC_LoadingScreen,				"LoadingScreens.dbc")
DBC_LOAD(DBC_Map,						"Map.dbc")
DBC_LOAD(DBC_TerrainType,				"TerrainType.dbc")
DBC_LOAD(DBC_WMOAreaTable,				"WMOAreaTable.dbc")
DBC_LOAD(DBC_WorldSafeLocs,				"WorldSafeLocs.dbc")

// Character
DBC_LOAD(DBC_CharacterFacialHairStyles,		"CharacterFacialHairStyles.dbc")
DBC_LOAD(DBC_CharHairGeosets,				"CharHairGeosets.dbc")
DBC_LOAD(DBC_CharSections,					"CharSections.dbc")
DBC_LOAD(DBC_ChrRaces,						"ChrRaces.dbc")

// DisplayInfo
DBC_LOAD(DBC_CreatureDisplayInfo,			"CreatureDisplayInfo.dbc")
DBC_LOAD(DBC_CreatureDisplayInfoExtra,		"CreatureDisplayInfoExtra.dbc")
DBC_LOAD(DBC_CreatureModelData,				"CreatureModelData.dbc")

void OpenDBs()
{
	DBC_AnimationData.Open();
	DBC_AreaTable.Open();
	DBC_CinematicCamera.Open();
	DBC_CinematicSequences.Open();
    DBC_GroundEffectDoodad.Open();
    DBC_GroundEffectTexture.Open();
    DBC_Light.Open();
    DBC_LightFloatBand.Open();
    DBC_LightIntBand.Open();
    DBC_LightParams.Open();
    DBC_LightSkybox.Open();
	DBC_LiquidMaterial.Open();
    DBC_LiquidType.Open();
    DBC_LoadingScreen.Open();
    DBC_Map.Open();
	DBC_TerrainType.Open();
    DBC_WMOAreaTable.Open();
	DBC_WorldSafeLocs.Open();
		
	// Character
	DBC_CharacterFacialHairStyles.Open();
	DBC_CharHairGeosets.Open();
	DBC_CharSections.Open();
	DBC_ChrRaces.Open();

	// DisplayInfo
	DBC_CreatureDisplayInfo.Open();
	DBC_CreatureDisplayInfoExtra.Open();
	DBC_CreatureModelData.Open();
}
