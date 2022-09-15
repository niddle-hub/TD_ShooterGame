#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryItem.generated.h"

class UInventoryItem;
class AEquipableItem;
class APickableItem;
class ATD_ShooterGameCharacter;

USTRUCT(BlueprintType)
struct FInventoryItemDescription : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct FWeaponTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<APickableItem> PickableActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AEquipableItem> EquipableActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FInventoryItemDescription WeaponDescription;
};


USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item view")
	TSubclassOf<APickableItem> PickableActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item view")
	TSubclassOf<UInventoryItem> InventoryItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item view")
	FInventoryItemDescription InventoryItemDescription;
};

UCLASS(Blueprintable)
class TD_SHOOTERGAME_API UInventoryItem : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(FName InItemID, const FInventoryItemDescription& In_ItemDescription);
	
	UFUNCTION(BlueprintCallable, Category = "Invenory Item")
	FName GetItemID() const { return ItemID; };

	UFUNCTION(BlueprintCallable, Category = "Invenory Item")
	const FInventoryItemDescription& GetItemDescription() const { return ItemDescription; };

	UFUNCTION(BlueprintCallable, Category = "Invenory Item")
	virtual bool IsEquipable() const { return bIsEquipable; };

	UFUNCTION(BlueprintCallable, Category = "Invenory Item")
	virtual bool IsConsumable() const { return bIsConsumable; };

	bool Consume(ATD_ShooterGameCharacter* Character);

protected:
	virtual bool InternalConsume(ATD_ShooterGameCharacter* ConsumeTarget) PURE_VIRTUAL(UInventoryItem::Consume, return false;);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Invenory Item")
	FName ItemID = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Invenory Item")
	FInventoryItemDescription ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Invenory Item")
	bool bIsEquipable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Invenory Item")
	bool bIsConsumable = false;

private:
	bool bIsInitialized = false;
	
};
