#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterInventoryComponent.generated.h"

class UInventoryViewWidget;
class UInventoryItem;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	DECLARE_DELEGATE(FOnInventorySlotUpdatedSignature)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UInventoryItem> ItemInSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 1;

	void BindInventorySlotUpdated(const FOnInventorySlotUpdatedSignature& Callback) const;
	void UnbindInventorySlotUpdated() const;
	void Update() const;
	void Clear();

private:
	mutable FOnInventorySlotUpdatedSignature OnInventorySlotUpdated;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TD_SHOOTERGAME_API UCharacterInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	void OpenInventory(APlayerController* PlayerController);
	void CloseInventory() const;
	bool IsInventoryOpen() const;

	bool AddItem(TWeakObjectPtr<UInventoryItem> ItemToAdd, int32 Quantity = 1);
	bool RemoveItem(FName ItemId);

	int32 GetInventorySize() const;
	bool HasFreeSlot() const;

	TArray<FInventorySlot> GetAllItemsCopy() const;
	TArray<FText> GetAllItemsNames() const;

protected:
	virtual void BeginPlay() override;

	void CreateViewWidget(APlayerController* PlayerController);

	FInventorySlot* GetInventorySlotByItemID(FName ItemID);
	FInventorySlot* FindFreeInventorySlot();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<UInventoryViewWidget> InventoryViewWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta=(ClampMin = 1, UIMin = 1))
	int32 Capacity = 16;

private:
	TWeakObjectPtr<UInventoryViewWidget> InventoryViewWidget;

	int32 ItemsInInventory = 0;
};
