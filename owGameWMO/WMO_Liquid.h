#pragma once

#include "WMO_Part_Material.h"

class CWMO_Liquid : public Liquid
{
public:
	CWMO_Liquid(uint32 _x, uint32 _y);

	void CreateFromWMO(std::shared_ptr<IFile> f, std::shared_ptr<const WMO_Part_Material> _material, const DBC_LiquidTypeRecord* _liquidType, bool _indoor);
};