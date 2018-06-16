#pragma once

#include "DBC__File.h"

DBC_DEF_BEGIN(DBC_LightParams)

__DBC_TVALUE(uint32, HighlightSky, 2);
__DBC_REF_ID(DBC_LightSkybox, LightSkyboxID, 3);

__DBC_TVALUE(float, Glow, 4);
__DBC_TVALUE(float, WaterShallowAlpha, 5);
__DBC_TVALUE(float, WaterDeepAlpha, 6);
__DBC_TVALUE(float, OceanShallowAlpha, 7);
__DBC_TVALUE(float, OceanDeepAlpha, 8);

__DBC_TVALUE(uint32, Flags, 9);

DBC_DEF_END