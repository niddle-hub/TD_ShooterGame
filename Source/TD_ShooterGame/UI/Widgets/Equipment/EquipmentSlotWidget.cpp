#include "EquipmentSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TD_ShooterGame/Utils/SGDataTableUtils.h"
#include "TD_ShooterGame/Inventory/Items/InventoryItem.h"
#include "TD_ShooterGame/Actors/Equipment/EquipableItem.h"
#include "TD_ShooterGame/UI/Widgets/Inventory/InventorySlotWidget.h"
#include "TD_ShooterGame/Inventory/Items/Weapons/WeaponInventoryItem.h"

void UEquipmentSlotWidget::InitializeSlot(const TWeakObjectPtr<AEquipableItem> Item, const int32 Index)
{
	if (!Item.IsValid())
	{
		return;
	}

	LinkedEquipableItem = Item;
	LinkedSlotIndex = Index;

	const FWeaponTableRow* WeaponData = SGDataTableUtils::FindData<FWeaponTableRow>(Item->GetDataTableID(), AssetPath::DataTableWeapons);
	if (WeaponData != nullptr)
	{
		AdapterLinkedInventoryItem = NewObject<UWeaponInventoryItem>(Item->GetOwner());
		AdapterLinkedInventoryItem->Initialize(Item->GetDataTableID(), WeaponData->WeaponDescription);
		AdapterLinkedInventoryItem->SetEquipWeaponClass(WeaponData->EquipableActorClass);
	}
}

void UEquipmentSlotWidget::Update() const
{
	ImageWeaponIcon->SetOpacity(0.f);
	ImageWeaponIcon->SetBrushFromTexture(nullptr);
	TextWeaponName->SetText(FText::FromString(TEXT("")));
	
	if (LinkedEquipableItem.IsValid())
	{
		ImageWeaponIcon->SetOpacity(1.f);
		ImageWeaponIcon->SetBrushFromTexture(AdapterLinkedInventoryItem->GetItemDescription().Icon);
		TextWeaponName->SetText(AdapterLinkedInventoryItem->GetItemDescription().Name);
	}
}

FReply UEquipmentSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!LinkedEquipableItem.IsValid())
	{
		return FReply::Handled();
	}

	FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return Reply.NativeReply;
}

void UEquipmentSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	checkf(DragNDropWidgetClass.Get() != nullptr, TEXT("UEquipmentSlotWidget::NativeOnDragDetected: DragNDropWidgetClass is not valid!"));

	if (!AdapterLinkedInventoryItem.IsValid())
	{
		return;
	}

	UDragDropOperation* DragDropOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	UInventorySlotWidget* DragWidget = CreateWidget<UInventorySlotWidget>(GetOwningPlayer(), DragNDropWidgetClass);
	
	DragWidget->SetItemIcon(AdapterLinkedInventoryItem->GetItemDescription().Icon);
	DragDropOperation->DefaultDragVisual = DragWidget;
	DragDropOperation->Pivot = EDragPivot::CenterCenter;
	DragDropOperation->Payload = AdapterLinkedInventoryItem.Get();
	OutOperation = DragDropOperation;

	LinkedEquipableItem.Reset();
	if (OnEquipmentRemovedFromSlot.IsBound())
	{
		OnEquipmentRemovedFromSlot.Execute(LinkedSlotIndex);
	}
	Update();
}

bool UEquipmentSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UWeaponInventoryItem* DroppedItem = Cast<UWeaponInventoryItem>(InOperation->Payload);
	if (IsValid(DroppedItem))
	{
		return OnEquipmentDropInSlot.Execute(DroppedItem->GetEquipWeaponClass(), LinkedSlotIndex);
	}
	return false;
}

void UEquipmentSlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	AdapterLinkedInventoryItem = Cast<UWeaponInventoryItem>(InOperation->Payload);
	// ReSharper disable once CppExpressionWithoutSideEffects
	OnEquipmentDropInSlot.Execute(AdapterLinkedInventoryItem->GetEquipWeaponClass(), LinkedSlotIndex);
}
