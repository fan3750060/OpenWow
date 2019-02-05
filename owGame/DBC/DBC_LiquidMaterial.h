#pragma once

#include "DBC__File.h"

DBC_DEF_BEGIN(DBC_LiquidMaterial)

__DBC_TVALUE(uint32, LiquidVertexFormat, 2);
__DBC_TVALUE(uint32, flags, 3);					// 	&1: isTransparent

DBC_DEF_END
