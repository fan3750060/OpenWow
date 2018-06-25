#pragma once

#include "M2_Types.h"

class CM2_Part_Material
{
public:
	CM2_Part_Material(const SM2_Material& _proto);

	void Set() const;

private:
	bool isUNLIT;
	bool isUNFOGGED;
	bool isTWOSIDED;
	bool isDEPTHTEST;
	bool isDEPTHWRITE;

	uint16 blending_mode;

	
};