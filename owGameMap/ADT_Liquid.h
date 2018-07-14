#pragma once

#include "ADT_MCNK.h"

class CADT_Liquid : public Liquid
{
public:
	CADT_Liquid(uint32 _x, uint32 _y);

	void CreateFromMCLQ(IFile* f, ADT_MCNK_Header header);
	void CreateFromTerrainMH2O(IFile* f, MH2O_Header* _header);
};