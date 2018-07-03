#pragma once

#include "DBC__File.h"

enum DBC_LIQUIDTYPE_Type : uint32_t
{
	lq_magma = 0,
	lq_slime = 2,
	lq_river = 3,
	lq_ocean = lq_river,
	lq_unknown
};

DBC_DEF_BEGIN(DBC_LiquidType)

__DBC_STRING(Name, 2);
__DBC_TVALUE(DBC_LIQUIDTYPE_Type, Type, 3);
//__DBC_REF_ID(DBC_Spell, SpellID, 4);

DBC_DEF_END