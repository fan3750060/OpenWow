#include "stdafx.h"

// General
#include "ItemTemplate.h"

ItemTemplate::ItemTemplate() :
	DisplayId(0),
	InventoryType(InventoryType::NON_EQUIP),
	EnchantAuraID(0)
{}

ItemTemplate::ItemTemplate(uint32 _displayId, InventoryType::List _inventoryType, uint32 _enchantAuraID) :
	DisplayId(_displayId),
	InventoryType(_inventoryType),
	EnchantAuraID(_enchantAuraID)
{}

ItemTemplate::ItemTemplate(const DBC_ItemDisplayInfoRecord* _display, InventoryType::List _inventoryType, uint32 _enchantAuraID) :
	DisplayId(0),
	InventoryType(InventoryType::NON_EQUIP),
	EnchantAuraID(0)
{
	if (_display == nullptr)
	{
		return;
	}

	DisplayId = _display->Get_ID();
	InventoryType = _inventoryType;
	EnchantAuraID = _enchantAuraID;
}

void ItemTemplate::TemplateSet(const ItemTemplate & _o)
{
	DisplayId = _o.DisplayId;
	InventoryType = _o.InventoryType;
	EnchantAuraID = _o.EnchantAuraID;
}

void ItemTemplate::TemplateFill(ByteBuffer& b)
{
	b.readBytes(&DisplayId, 4);
	b.readBytes(&InventoryType, 1);
	b.readBytes(&EnchantAuraID, 4);
}

void ItemTemplate::TemplatePrint()
{
	Log::Print("Item(%d, (InventoryType::List) %d, %d);", DisplayId, InventoryType, EnchantAuraID);
}