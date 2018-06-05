#include "stdafx.h"

// General
#include "ADT_Liquid.h"

CADT_Liquid::CADT_Liquid(uint32 _x, uint32 _y) :
	Liquid(_x, _y)
{}

void CADT_Liquid::CreateFromMCLQ(IFile* f, ADT_MCNK_Header header)
{
	ydir = 1.0f;

	initGeometry(f);

	if (header.flags.lq_river)
	{
		InitTextures(DBC_LIQUIDTYPE_Type::lq_river);
	}
	else if (header.flags.lq_ocean)
	{
		InitTextures(DBC_LIQUIDTYPE_Type::lq_ocean);
	}
	else if (header.flags.lq_magma)
	{
		InitTextures(DBC_LIQUIDTYPE_Type::lq_magma);
	}
	else if (header.flags.lq_slime)
	{
		InitTextures(DBC_LIQUIDTYPE_Type::lq_slime);
	}

}
