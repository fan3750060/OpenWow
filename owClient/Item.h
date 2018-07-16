#pragma once

#include "CharacterEnums.h"


#include "ItemTemplate.h"
#include "MeshIDEnums.h"


class Item : public ItemTemplate
{
public:
	Item();

	void Init(ItemTemplate& _template);

private:

public:
	string					gender;
};