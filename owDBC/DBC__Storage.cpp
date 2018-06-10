#include "stdafx.h"

// General
#include "DBC__Storage.h"

// Tables

DBC_LOAD(DBÑ_AnimationData, "AnimationData.dbc")
DBC_LOAD(DBÑ_AreaTable, "AreaTable.dbc")
DBC_LOAD(DBC_GroundEffectDoodad, "GroundEffectDoodad.dbc")
DBC_LOAD(DBC_GroundEffectTexture, "GroundEffectTexture.dbc")
DBC_LOAD(DBC_Light, "Light.dbc")
DBC_LOAD(DBC_LightFloatBand, "LightFloatBand.dbc")
DBC_LOAD(DBC_LightIntBand, "LightIntBand.dbc")
DBC_LOAD(DBC_LightParams, "LightParams.dbc")
DBC_LOAD(DBC_LightSkybox, "LightSkybox.dbc")
DBC_LOAD(DBC_LiquidType, "LiquidType.dbc")
DBC_LOAD(DBC_LoadingScreen, "LoadingScreens.dbc")
DBC_LOAD(DBC_Map, "Map.dbc")
DBC_LOAD(DBC_TerrainType, "TerrainType.dbc")
DBC_LOAD(DBC_WMOAreaTable, "WMOAreaTable.dbc")


void OpenDBs()
{
	DBÑ_AnimationData.Open();
	DBÑ_AreaTable.Open();
    DBC_GroundEffectDoodad.Open();
    DBC_GroundEffectTexture.Open();
    DBC_Light.Open();
    DBC_LightFloatBand.Open();
    DBC_LightIntBand.Open();
    DBC_LightParams.Open();
    DBC_LightSkybox.Open();
    DBC_LiquidType.Open();
    DBC_LoadingScreen.Open();
    DBC_Map.Open();
	DBC_TerrainType.Open();
    DBC_WMOAreaTable.Open();
}

bool DBC__Storage::Init()
{
	return true;
}

void DBC__Storage::Destroy()
{

}
