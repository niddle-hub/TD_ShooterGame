#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TD_ShooterGame/TD_ShooterGameTypes.h"
#include "TD_ShooterGame/Subsystems/SaveSubsystem/SaveSubsystemInterface.h"
#include "CharacterEquipmentComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquippedWeaponAmmoChangedSignature, int32, int32);
DECLARE_MULTICAST_DELEGATE(FOnWeaponChangedSignature);

class AEquipableItem;
class ARangeWeaponItem;
class UEquipmentViewWidget;
class ATD_ShooterGameCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TD_SHOOTERGAME_API UCharacterEquipmentComponent : public UActorComponent, public ISaveSubsystemInterface
{
	GENERATED_BODY()

public:
	FOnEquippedWeaponAmmoChangedSignature OnEquippedWeaponAmmoChangedDelegate;
	FOnWeaponChangedSignature OnWeaponChangedDelegate;
	
	FORCEINLINE AEquipableItem* GetEquippedItem() const { return EquippedItem.Get(); }
	FORCEINLINE ARangeWeaponItem* GetEquippedRangeWeapon() const { return EquippedWeapon.Get(); }
	
	void ReloadEquippedWeapon() const;
	void ReloadAmmoInEquippedWeapon(int32 NumberOfAmmo = 0, bool CheckIsFull = false);
	
	AEquipableItem* GetItemFromSlot(const EEquipmentSlot Slot) const;
	AEquipableItem* GetItemFromSlot(const int32 SlotIndex) const;
	
	bool IsSlotEmpty(const EEquipmentSlot Slot) const;
	bool IsSlotEmpty(const int32 SlotIndex) const;
	
	bool IsEnabledAutoEquip() const { return bAutoEquip; }
	bool AddEquipmentItemInSlot(const TSubclassOf<AEquipableItem> ItemClass, EEquipmentSlot Slot);
	bool AddEquipmentItemInSlot(const TSubclassOf<AEquipableItem> ItemClass, int32 SlotIndex);

	bool EquipItemInSlot(EEquipmentSlot Slot);
	void RemoveItemFromSlot(int32 SlotIndex);

	void EquipNext();
	void EquipPrevious();
	void UnequipCurrentItem();
	void OnNewItemEquippedInSlot() const;
	bool IsEquipping() const { return bIsEquipping; }
	const TArray<AEquipableItem*>& GetEquipmentItems() const { return ItemsArray; }
	EEquipableItemType GetEquippedItemType() const;
	
	void OpenEquipmentMenu(APlayerController* PlayerController);
	void CloseEquipmentMenu() const;
	bool IsEquipmentMenuOpened() const;
	
	void AddAmmunition(EAmmunitionType AmmoType, int32 NumberOfAmmo);
	int32 GetMaxAmmunitionAmountForType(EAmmunitionType AmmoType) const;

/** ISaveSubsystemInterface **/
	virtual void OnLevelDeserialized_Implementation() override;
/** ~ISaveSubsystemInterface **/

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TMap<EAmmunitionType, int32> MaxAmmunitionAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TMap<EAmmunitionType, int32> StartUpAmmunitionAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TMap<EEquipmentSlot, TSubclassOf<AEquipableItem>> ItemsLoadout;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSet<EEquipmentSlot> IgnoreSlotsWhileSwitching;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	bool bAutoEquip = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|View")
	TSubclassOf<UEquipmentViewWidget> ViewWidgetClass;

	void CreateEquipmentWidget(APlayerController* PlayerController);

private:
	TWeakObjectPtr<ATD_ShooterGameCharacter> OwnerCharacter;

	UPROPERTY(SaveGame)
	TArray<int32> AmmunitionArray;
	
	UPROPERTY(SaveGame)
	TArray<AEquipableItem*> ItemsArray;

	uint32 NextItemsArraySlotIndex(uint32 CurrentSlotIndex) const;
	uint32 PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex) const;

	void OnWeaponChanged() const;
	void CreateLoadout();

	FDelegateHandle OnEquippedWeaponAmmoChangedHandle;
	FDelegateHandle OnEquippedWeaponReloadedHandle;
	
	UPROPERTY(SaveGame)
	EEquipmentSlot EquippedSlot;
	EEquipmentSlot PreviousEquippedSlot;
	
	TWeakObjectPtr<AEquipableItem> EquippedItem;
	TWeakObjectPtr<ARangeWeaponItem> EquippedWeapon;

	UFUNCTION()
	void OnEquippedWeaponAmmoChanged(int32 NewAmmo) const;
	void UpdateAmmo() const;

	void BroadcastOnWeaponChanged() const;

	UFUNCTION()
	void OnEquippedWeaponReloaded();
	
	int32 GetAvailableAmmunitionForEquippedWeapon() const;
	
	bool bIsEquipping = false;
	
	FTimerHandle EquipTimerHandle;

	TWeakObjectPtr<UEquipmentViewWidget> ViewWidget;
};
