#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "EquipmentSlotWidget.generated.h"

class UImage;
class UTextBlock;
class AEquipableItem;
class UInventorySlotWidget;
class UWeaponInventoryItem;

UCLASS()
class TD_SHOOTERGAME_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeSlot(TWeakObjectPtr<AEquipableItem> Item, int32 Index);
	void Update() const;
	
	DECLARE_DELEGATE_RetVal_TwoParams(bool, FOnEquipmentDropInSlotSignature, const TSubclassOf<AEquipableItem>&, int32);
	DECLARE_DELEGATE_OneParam(FOnEquipmentRemovedFromSlotSignature, int32);

	FOnEquipmentDropInSlotSignature OnEquipmentDropInSlot;
	FOnEquipmentRemovedFromSlotSignature OnEquipmentRemovedFromSlot;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* ImageWeaponIcon;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* TextWeaponName;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> DragNDropWidgetClass;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	TWeakObjectPtr<AEquipableItem> LinkedEquipableItem;
	TWeakObjectPtr<UWeaponInventoryItem> AdapterLinkedInventoryItem;
	int32 LinkedSlotIndex = 0;
};
