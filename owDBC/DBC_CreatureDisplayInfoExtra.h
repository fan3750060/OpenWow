#pragma once

#include "DBC__File.h"

DBC_DEF_BEGIN(DBC_CreatureDisplayInfoExtra)

__DBC_TVALUE(uint32, RaceID, 2);
__DBC_REF_ID(DBC_ChrRaces, Race, 2);
__DBC_TVALUE(uint32, Gender, 3);

__DBC_TVALUE(uint32, SkinColor, 4);
__DBC_TVALUE(uint32, FaceType, 5);
__DBC_TVALUE(uint32, HairType, 6);
__DBC_TVALUE(uint32, HairStyleOrColor, 7);
__DBC_TVALUE(uint32, BeardStyle, 8);

__DBC_TVALUE(uint32, Helm, 9);
__DBC_TVALUE(uint32, Shoulder, 10);
__DBC_TVALUE(uint32, Shirt, 11);
__DBC_TVALUE(uint32, Cuirass, 12);
__DBC_TVALUE(uint32, Belt, 13);
__DBC_TVALUE(uint32, Legs, 14);
__DBC_TVALUE(uint32, Boots, 15);
__DBC_TVALUE(uint32, Wrist, 16);
__DBC_TVALUE(uint32, Gloves, 17);
__DBC_TVALUE(uint32, Tabard, 18);
__DBC_TVALUE(uint32, Cape, 19);
__DBC_TVALUE(uint32, CanEquip, 20);

__DBC_STRING(Texture, 21);

DBC_DEF_END
