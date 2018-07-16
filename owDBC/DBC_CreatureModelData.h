#pragma once

#include "DBC__File.h"

DBC_DEF_BEGIN(DBC_CreatureModelData)

__DBC_TVALUE(uint32,  Flags,                   2);
__DBC_STRING(         ModelPath,               3);
__DBC_TVALUE(uint32,  SizeClass,               4);
__DBC_TVALUE(float_t, Scale,                   5);

__DBC_TVALUE(uint32,  BloodLevelID,            6);

__DBC_TVALUE(uint32,  FootprintID,             7);
__DBC_TVALUE(float_t, FootprintTextureLength,  8);	// most time 18.0
__DBC_TVALUE(float_t, FootprintTextureWidth,   9);	// mostly 12, others are 0.0 - 20.0
__DBC_TVALUE(float_t, FootprintParticleScale,  10);	// mostly 1.0, others are 0.0 - 5.0

__DBC_TVALUE(uint32,  FootstepShakeSizeID,     12);
__DBC_TVALUE(uint32,  DeathThudShakeSizeID,    13);
__DBC_TVALUE(uint32,  SoundDataID,             14);

__DBC_TVALUE(float_t, CollisionWidth,          15);
__DBC_TVALUE(float_t, CollisionHeight,         16);
__DBC_TVALUE(float_t, MountHeight,             17);

__DBC_TARRAY(float_t, GeoBoxMin,               18, 3);
__DBC_TARRAY(float_t, GeoBoxMax,               21, 3);

__DBC_TVALUE(float_t, WorldEffectScale,        24);
__DBC_TVALUE(float_t, AttachedEffectScale,     25);
__DBC_TVALUE(float_t, MissileCollisionRadius,  26);
__DBC_TVALUE(float_t, MissileCollisionPush,    27);
__DBC_TVALUE(float_t, MissileCollisionRaise,   28);

DBC_DEF_END
