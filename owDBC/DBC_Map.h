#pragma once

#include "DBC__File.h"

DBC_DEF_BEGIN(DBC_Map)

__DBC_STRING(Directory, 2);								// string
__DBC_TVALUE(uint32, InstanceType, 3);				// uint (0-none, 1-party, 2-raid, 4-arena, >=5-none)
__DBC_TVALUE(uint32, MapType, 4);					// uint (1-normal, 2-test, 3-internal)

__DBC_STRING(Name, 5);
__DBC_TVALUE(uint32_t, MinLevel, 6);
__DBC_TVALUE(uint32_t, MaxLevel, 7);
__DBC_TVALUE(uint32_t, MaxPlayers, 8);

// 9, 10, 11 ???

// 12 parent MapID

__DBC_LOCSTR(MapDescriptionAllicane, 13);			// loc
__DBC_LOCSTR(MapDescriptionHorde, 14);				// loc

__DBC_TVALUE(uint32, LoadingScreenID, 15);			// uint [LoadingScreen]

DBC_DEF_END