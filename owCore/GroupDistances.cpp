#include "stdafx.h"

// General
#include "GroupDistances.h"

CGroupDistances::CGroupDistances()
{
	_Bindings->RegisterUpdatableObject(this);
}

CGroupDistances::~CGroupDistances()
{
	_Bindings->UnregisterUpdatableObject(this);
}

void CGroupDistances::InitDefault()
{
	/*ADT_MCNK_Distance = 998.0f;
	ADT_MDX_Distance = 384.0f;
	WMO_MODD_Distance = 64.0f;
	ADT_MCNK_HighRes_Distance = 384.0f;
	fogdistance = 51200.0f;*/

	ADT_MCNK_Distance = 998.0f ;
	ADT_MCNK_HighRes_Distance = 384.0f;
	ADT_MDX_Distance = 384.0f ;
	ADT_WMO_Distance = ADT_MDX_Distance * 1.5f;
	WMO_MODD_Distance = 64.0f ;

	// DEBUG
	ADT_MCNK_Distance *= 2;
	ADT_MCNK_HighRes_Distance *= 0.75f;
	ADT_MDX_Distance *= 2;
	ADT_WMO_Distance *= 3;
	WMO_MODD_Distance *= 5;
}
