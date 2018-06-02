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
	mapdrawdistance = 998.0f * 12.0f;
	modeldrawdistance = 384.0f * 12.0f;
	doodaddrawdistance = 64.0f * 12.0f;
	highresdistance = 384.0f * 12.0f;
	fogdistance = 51200.0f;
}
