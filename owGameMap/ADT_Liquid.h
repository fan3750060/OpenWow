#pragma once

#include "ADT_MCNK.h"

class CADT_Liquid : public Liquid
{
public:
	CADT_Liquid(uint32 _x, uint32 _y);

	void CreateFromMCLQ(std::shared_ptr<IFile>, ADT_MCNK_Header header);
};