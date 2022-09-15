#include "EquipmentViewWidget.h"

#include "EquipmentSlotWidget.h"
#include "Components/HorizontalBox.h"
#include "TD_ShooterGame/Actors/Equipment/EquipableItem.h"
#include "TD_ShooterGame/Components/CharacterComponents/CharacterEquipmentComponent.h"

void UEquipmentViewWidget::InitializeWidget(UCharacterEquipmentComponent* EquipmentComponent)
{
	LinkedEquipmentComponent = EquipmentComponent;
	const TArray<AEquipableItem*>& Equipment = LinkedEquipmentComponent->GetEquipmentItems();
	for(int32 Index = 1; Index < Equipment.Num(); ++Index)
	{
		AddItemSlotView(Equipment[Index], Index);
	}
}

void UEquipmentViewWidget::AddItemSlotView(AEquipableItem* LinkToItem, const int32 SlotIndex)
{
	checkf(IsValid(DefaultSlotViewClass.Get()), TEXT("UEquipmentViewWidget::AddItemSlotView: DefaultSlotViewClass is not valid"));
	UEquipmentSlotWidget* NewSlot = CreateWidget<UEquipmentSlotWidget>(this, DefaultSlotViewClass);
	if (IsValid(NewSlot))
	{
		NewSlot->InitializeSlot(LinkToItem, SlotIndex);
		ItemSlotsBox->AddChildToHorizontalBox(NewSlot);
		NewSlot->Update();
		NewSlot->OnEquipmentDropInSlot.BindUObject(this, &UEquipmentViewWidget::EquipItemInSlot);
		NewSlot->OnEquipmentRemovedFromSlot.BindUObject(this, &UEquipmentViewWidget::RemoveItemFromSlot);
	}
}

void UEquipmentViewWidget::UpdateSlot(const int32 SlotIndex) const
{
	UEquipmentSlotWidget* WidgetToUpdate = Cast<UEquipmentSlotWidget>(ItemSlotsBox->GetChildAt(SlotIndex - 1));
	if (IsValid(WidgetToUpdate))
	{
		WidgetToUpdate->InitializeSlot(LinkedEquipmentComponent->GetEquipmentItems()[SlotIndex], SlotIndex);
		WidgetToUpdate->Update();
	}
}

bool UEquipmentViewWidget::EquipItemInSlot(const TSubclassOf<AEquipableItem>& ItemClass, const int32 SlotIndex) const
{
	return LinkedEquipmentComponent->AddEquipmentItemInSlot(ItemClass, SlotIndex);
}

void UEquipmentViewWidget::RemoveItemFromSlot(const int32 SlotIndex) const
{
	LinkedEquipmentComponent->RemoveItemFromSlot(SlotIndex);
}
