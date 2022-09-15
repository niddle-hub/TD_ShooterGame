#include "InventoryViewWidget.h"

#include "InventorySlotWidget.h"
#include "Components/GridPanel.h"
#include "TD_ShooterGame/Components/CharacterComponents/CharacterInventoryComponent.h"

void UInventoryViewWidget::InitializeWidget(TArray<FInventorySlot>& InventorySlots)
{
	for (FInventorySlot& InventorySlot : InventorySlots)
	{
		AddInventorySlotWidget(InventorySlot);
	}
}

void UInventoryViewWidget::AddInventorySlotWidget(FInventorySlot& NewSlot)
{
	checkf(InventorySlotWidgetClass.Get() != nullptr, TEXT("UInventoryViewWidget::AddInventorySlotWidget: InventorySlotWidgetClass is nullptr"));

	UInventorySlotWidget* InventorySlotWidget = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
	if (InventorySlotWidget != nullptr)
	{
		InventorySlotWidget->InitializeItemSlot(NewSlot);

		const int32 CurrentSlotCount = InventoryGridPanel->GetChildrenCount();
		const int32 CurrentSlotRow = CurrentSlotCount / ColumnsCount;
		const int32 CurrentSlotColumn = CurrentSlotCount % ColumnsCount;

		InventoryGridPanel->AddChildToGrid(InventorySlotWidget, CurrentSlotColumn, CurrentSlotRow);
		InventorySlotWidget->Update();
	}
}
