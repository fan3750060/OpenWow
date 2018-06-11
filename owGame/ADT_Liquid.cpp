#include "stdafx.h"

// General
#include "ADT_Liquid.h"

CADT_Liquid::CADT_Liquid(uint32 _x, uint32 _y) :
	Liquid(_x, _y)
{}

void CADT_Liquid::CreateFromMCLQ(IFile* f, ADT_MCNK_Header header)
{
	ydir = 1.0f;

	initGeometry(header.getLiquidType(), f);
	InitTextures(header.getLiquidType());

}
