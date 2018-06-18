#pragma once

#include "DBC__Forwards.h"


// My files
#include "DBC_AnimationData.h"
#include "DBC_AreaTable.h"
#include "DBC_CinematicCamera.h"
#include "DBC_CinematicSequences.h"
#include "DBC_GroundEffectDoodad.h"
#include "DBC_GroundEffectTexture.h"
#include "DBC_Light.h"
#include "DBC_LightFloatBand.h"
#include "DBC_LightIntBand.h"
#include "DBC_LightParams.h"
#include "DBC_LightSkyBox.h"
#include "DBC_LiquidType.h"
#include "DBC_LoadingScreen.h"
#include "DBC_Map.h"
#include "DBC_TerrainType.h"
#include "DBC_WMOAreaTable.h"

 void OpenDBs();


class DBC__Storage
{
public:
	bool Init();
	void Destroy();
};
