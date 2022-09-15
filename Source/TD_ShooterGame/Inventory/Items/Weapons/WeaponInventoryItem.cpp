#include "WeaponInventoryItem.h"

#include "TD_ShooterGame/TD_ShooterGameCharacter.h"

UWeaponInventoryItem::UWeaponInventoryItem()
{
	bIsConsumable = true;
}

bool UWeaponInventoryItem::InternalConsume(ATD_ShooterGameCharacter* ConsumeTarget)
{
	return ConsumeTarget->EquipItemFromInventory(this);
}
