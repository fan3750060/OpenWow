#pragma once

#include "DBC__File.h"

DBC_DEF_BEGIN(DBÑ_AnimationData)

__DBC_LOCSTR(							Name,				2);
__DBC_TVALUE(uint32,					WeaponFlags,		3);
__DBC_TVALUE(uint32,					BodyFlags,			4);
__DBC_TVALUE(uint32,					Flags,				5);

__DBC_REF_ID(DBÑ_AnimationData,			Fallback,			6);
__DBC_REF_ID(DBÑ_AnimationData,			Previos,			7);

DBC_DEF_END