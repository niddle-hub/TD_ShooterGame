#include "InventorySlotWidget.h"

#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TD_ShooterGame/TD_ShooterGameCharacter.h"
#include "TD_ShooterGame/Inventory/Items/InventoryItem.h"
#include "TD_ShooterGame/Components/CharacterComponents/CharacterInventoryComponent.h"

void UInventorySlotWidget::InitializeItemSlot(FInventorySlot& SlotContainer)
{
	LinkedSlot = &SlotContainer;

	FInventorySlot::FOnInventorySlotUpdatedSignature OnInventorySlotUpdated;
	OnInventorySlotUpdated.BindUObject(this, &UInventorySlotWidget::Update);
	LinkedSlot->BindInventorySlotUpdated(OnInventorySlotUpdated);
}

void UInventorySlotWidget::Update() const
{
	ItemIcon->SetBrushFromTexture(nullptr);
	
	if (LinkedSlot == nullptr)
	{
		return;
	}

	if (LinkedSlot->ItemInSlot.IsValid())
	{
		const FInventoryItemDescription& ItemDescription = LinkedSlot->ItemInSlot->GetItemDescription();
		SetItemIcon(ItemDescription.Icon);
	}
}

void UInventorySlotWidget::SetItemIcon(const TSoftObjectPtr<UTexture2D> In_Icon) const
{
	if (In_Icon.IsValid())
	{
		ItemIcon->SetBrushFromTexture(In_Icon.Get());
	}
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (LinkedSlot == nullptr)
	{
		return FReply::Handled();
	}
	if (!LinkedSlot->ItemInSlot.IsValid())
	{
		return FReply::Handled();
	}

	const auto MouseButton = InMouseEvent.GetEffectingButton();
	if (MouseButton == EKeys::RightMouseButton)
	{
		const TWeakObjectPtr<UInventoryItem> LinkedSlotItem = LinkedSlot->ItemInSlot;
		ATD_ShooterGameCharacter* PlayerCharacter = Cast<ATD_ShooterGameCharacter>(LinkedSlotItem->GetOuter());
		if (LinkedSlotItem->Consume(PlayerCharacter))
		{
			LinkedSlot->Clear();
		}
		return FReply::Handled();
	}
	FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return Reply.NativeReply;
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UDragDropOperation*	DragOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	UInventorySlotWidget* DragWidget = CreateWidget<UInventorySlotWidget>(GetOwningPlayer(), GetClass());

	if (LinkedSlot->ItemInSlot->GetItemDescription().Icon.IsValid())
	{
		DragWidget->ItemIcon->SetBrushFromTexture(LinkedSlot->ItemInSlot->GetItemDescription().Icon.Get());
	}
	DragOperation->DefaultDragVisual = DragWidget;
	DragOperation->Pivot = EDragPivot::MouseDown;
	DragOperation->Payload = LinkedSlot->ItemInSlot.Get();
	OutOperation = DragOperation;
	
	LinkedSlot->Clear();
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	LinkedSlot->ItemInSlot = TWeakObjectPtr<UInventoryItem>(Cast<UInventoryItem>(InOperation->Payload));
	LinkedSlot->Update();
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!LinkedSlot->ItemInSlot.IsValid())
	{
		LinkedSlot->ItemInSlot = TWeakObjectPtr<UInventoryItem>(Cast<UInventoryItem>(InOperation->Payload));
		LinkedSlot->Update();
		return true;
	}
	return false;
}