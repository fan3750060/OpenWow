#include "stdafx.h"

// General
#include "Item.h"

Item::Item() 
{}

void Item::Init(ItemTemplate& _template)
{
	TemplateSet(_template);
}