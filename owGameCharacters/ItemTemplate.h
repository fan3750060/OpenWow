#pragma once

#include __PACK_BEGIN
struct ItemTemplate
{
	ItemTemplate();
	ItemTemplate(uint32	_displayId, InventoryType::List _inventoryType, uint32 _enchantAuraID);
	ItemTemplate(std::shared_ptr<const DBC_ItemDisplayInfoRecord> _display, InventoryType::List _inventoryType, uint32 _enchantAuraID);

	void TemplateSet(const ItemTemplate& _o);
	void TemplateFill(CByteBuffer& b);

	void TemplatePrint();

	uint32					DisplayId;
	InventoryType::List		InventoryType;
	uint32					EnchantAuraID;
};
#include __PACK_END