#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentViewWidget.generated.h"

class UHorizontalBox;
class AEquipableItem;
class UEquipmentSlotWidget;
class UCharacterEquipmentComponent;

UCLASS()
class TD_SHOOTERGAME_API UEquipmentViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeWidget(UCharacterEquipmentComponent* EquipmentComponent);
	void UpdateSlot(int32 SlotIndex) const;

protected:
	void AddItemSlotView(AEquipableItem* LinkToItem, int32 SlotIndex);

	bool EquipItemInSlot(const TSubclassOf<AEquipableItem>& ItemClass, int32 SlotIndex) const;
	void RemoveItemFromSlot(int32 SlotIndex) const;

	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* ItemSlotsBox;

	UPROPERTY(EditDefaultsOnly, Category = "EquipmentView")
	TSubclassOf<UEquipmentSlotWidget> DefaultSlotViewClass;

	TWeakObjectPtr<UCharacterEquipmentComponent> LinkedEquipmentComponent;
};
