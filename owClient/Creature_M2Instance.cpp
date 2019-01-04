#include "stdafx.h"

// General
#include "Creature_M2Instance.h"

Creature_M2Instance::Creature_M2Instance() : 
	CM2_Base_Instance(nullptr)
{}

Creature_M2Instance::~Creature_M2Instance()
{}

bool Creature_M2Instance::isMeshEnabled(uint32 _index) const
{
	uint32 div100 = _index / 100;
	uint32 mod100 = _index % 100;
	
	return (div100 == 0) && (mod100 == 0);
	//return true;
}