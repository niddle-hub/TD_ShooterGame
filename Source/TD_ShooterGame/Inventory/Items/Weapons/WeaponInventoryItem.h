#pragma once

#include "CoreMinimal.h"
#include "TD_ShooterGame/Inventory/Items/InventoryItem.h"
#include "WeaponInventoryItem.generated.h"

UCLASS()
class TD_SHOOTERGAME_API UWeaponInventoryItem : public UInventoryItem
{
	GENERATED_BODY()

public:
	UWeaponInventoryItem();

	virtual bool InternalConsume(ATD_ShooterGameCharacter* ConsumeTarget) override;
	
	void SetEquipWeaponClass(const TSubclassOf<AEquipableItem> NewWeaponClass) { EquipWeaponClass = NewWeaponClass; };
	TSubclassOf<AEquipableItem> GetEquipWeaponClass() const { return EquipWeaponClass; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponInventoryItem")
	TSubclassOf<AEquipableItem> EquipWeaponClass;
	
};
