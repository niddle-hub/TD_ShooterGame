#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TD_ShooterGame/TD_ShooterGameTypes.h"
#include "EquipableItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEqupementStateChangedSignature, bool, bIsEquipped);

UCLASS(Abstract, NotBlueprintable)
class TD_SHOOTERGAME_API AEquipableItem : public AActor
{
	GENERATED_BODY()
public:
	EEquipableItemType GetItemType() const { return ItemType; }

	UAnimMontage* GetCharacterEquipAnimation() const { return EquipAnimation; }

	virtual void Equip();
	virtual void Unequip();

	virtual void Hide() const {};
	virtual void UnHide() const {};
	
	FORCEINLINE bool IsEquipped() const { return bIsEquipped; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FName DataTableID = NAME_None;

	FName GetDataTableID() const { return DataTableID; }

	bool IsSlotCompatible(EEquipmentSlot Slot) const;
	EEquipmentSlot GetPrioritySlot() const;
	const TArray<EEquipmentSlot>& GetCompatibleSlots() const { return CompatibleSlots; }
	
protected:
	UPROPERTY(BlueprintAssignable)
	FOnEqupementStateChangedSignature OnEquipmentStateChangedDelegate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipableItem")
	EEquipableItemType ItemType = EEquipableItemType::EIT_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipableItem")
	UAnimMontage* EquipAnimation = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipableItem")
	TArray<EEquipmentSlot> CompatibleSlots;

	EEquipmentSlot PrioritySlot = EEquipmentSlot::ES_None;

	void OnEquippedStateChanged(bool NewState);

private:
	bool bIsEquipped = false;

};
