#include "stdafx.h"

// General
#include "DBC__Storage.h"

// Tables

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
DBC_LOAD(DBC_WMOAreaTable, "WMOAreaTable.dbc")


void OpenDBs()
{
	assert1(DBÑ_AreaTable.Open());
    assert1(DBC_GroundEffectDoodad.Open());
    assert1(DBC_GroundEffectTexture.Open());
    assert1(DBC_Light.Open());
    assert1(DBC_LightFloatBand.Open());
    assert1(DBC_LightIntBand.Open());
    assert1(DBC_LightParams.Open());
    assert1(DBC_LightSkybox.Open());
    assert1(DBC_LiquidType.Open());
    assert1(DBC_LoadingScreen.Open());
    assert1(DBC_Map.Open());
    assert1(DBC_WMOAreaTable.Open());
}

bool DBC__Storage::Init()
{
	return true;
}

void DBC__Storage::Destroy()
{

}
