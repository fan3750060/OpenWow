#pragma once

// =================================================================================================
// Build settings
// =================================================================================================

#define MDX_PARTICLES_ENABLE

#define CLASSIC    0
#define BC         1
#define WOTLK      2
#define CATACLYSM  3

#define CURRENT    CLASSIC

#define VERSION_Vanila  0
#define VERSION_BC		1
#define VERSION_WotLK	2

#define VERSION VERSION_WotLK

// Interfaces
#include "SkyManager.h"
#include "Managers.h"

// Wow types
#include "WowTypes.h"
#include "WowConsts.h"
#include "WowTime.h"
#include "WowChunkUtils.h"

#include "Function.h"
#include "FunctionWA.h"
#include "FunctionSA.h"