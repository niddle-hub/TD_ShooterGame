#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TD_ShooterGame/UI/Widgets/Equipment/EquipmentSlotWidget.h"
#include "InventorySlotWidget.generated.h"

class UImage;
struct FInventorySlot;

UCLASS()
class TD_SHOOTERGAME_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeItemSlot(FInventorySlot& SlotContainer);
	void Update() const;
	void SetItemIcon(TSoftObjectPtr<UTexture2D> In_Icon) const;

protected:
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	FInventorySlot* LinkedSlot;
};
