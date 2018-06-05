#pragma once

#include "WMO_Material.h"

class CWMO_Liquid : public Liquid
{
public:
	CWMO_Liquid(uint32 _x, uint32 _y);

	void CreateFromWMO(IFile* f, WMOMaterial* _material, const DBC_LiquidTypeRecord* _liquidType, bool _indoor);
};