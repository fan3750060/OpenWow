#pragma once

#include "Creature.h"

#include "CharacterEnums.h"

class CreatureEx : public Creature
{
public:
	CreatureEx();

private:
	Race::List Race;
	Gender::List Gender;
};