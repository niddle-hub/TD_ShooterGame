#include "EquipableItem.h"

void AEquipableItem::Equip()
{
	OnEquippedStateChanged(true);
}

void AEquipableItem::Unequip()
{
	OnEquippedStateChanged(false);
	Hide();
}

bool AEquipableItem::IsSlotCompatible(EEquipmentSlot Slot) const
{
	return CompatibleSlots.Contains(Slot);
}

EEquipmentSlot AEquipableItem::GetPrioritySlot() const
{
	if (PrioritySlot != EEquipmentSlot::ES_None)
	{
		return PrioritySlot;
	}
	return CompatibleSlots[0];
}

void AEquipableItem::OnEquippedStateChanged(bool NewState)
{
	bIsEquipped = NewState;
	
	if (OnEquipmentStateChangedDelegate.IsBound())
	{
		OnEquipmentStateChangedDelegate.Broadcast(bIsEquipped);
	}
}
