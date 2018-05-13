#pragma once

#include "DBC__File.h"

enum DBC_LIQUIDTYPE_Type : uint32_t
{
	lq_magma = 0,
	lq_slime = 2,
	lq_river = 3,
	lq_ocean = lq_river
};

DBC_DEF_BEGIN(DBC_LiquidType)

__DBC_STRING(Name, 2);
__DBC_TVALUE(DBC_LIQUIDTYPE_Type, Type, 3);			// 0: Water, 1: Ocean, 2: Magma, 3: Slime
// 4 Spell.dbc irefID

DBC_DEF_END