#include "CharacterInventoryComponent.h"
#include "TD_ShooterGame/Inventory/Items/InventoryItem.h"
#include "TD_ShooterGame/UI/Widgets/Inventory/InventoryViewWidget.h"

void FInventorySlot::BindInventorySlotUpdated(const FOnInventorySlotUpdatedSignature& Callback) const
{
	OnInventorySlotUpdated = Callback;
}

void FInventorySlot::UnbindInventorySlotUpdated() const
{
	OnInventorySlotUpdated.Unbind();
}

void FInventorySlot::Update() const
{
	if (OnInventorySlotUpdated.IsBound())
	{
		OnInventorySlotUpdated.Execute();
	}
}

void FInventorySlot::Clear()
{
	ItemInSlot = nullptr;
	Quantity = 0;
	Update();
}

void UCharacterInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InventorySlots.AddDefaulted(Capacity);
}

void UCharacterInventoryComponent::OpenInventory(APlayerController* PlayerController)
{
	if (!InventoryViewWidget.IsValid())
	{
		CreateViewWidget(PlayerController);
	}

	if (!InventoryViewWidget->IsVisible())
	{
		InventoryViewWidget->AddToViewport();
	}
}

void UCharacterInventoryComponent::CloseInventory() const
{
	if (InventoryViewWidget->IsVisible())
	{
		InventoryViewWidget->RemoveFromParent();
	}
}

bool UCharacterInventoryComponent::IsInventoryOpen() const
{
	bool Result = false;

	if (InventoryViewWidget.IsValid())
	{
		Result = InventoryViewWidget->IsVisible();
	}

	return Result;
}

bool UCharacterInventoryComponent::AddItem(TWeakObjectPtr<UInventoryItem> ItemToAdd, int32 Quantity)
{
	if (!ItemToAdd.IsValid() || Quantity <= 0)
	{
		return false;
	}

	bool Result = false;
	
	FInventorySlot* Slot = FindFreeInventorySlot();
	if (Slot != nullptr)
	{
		Slot->ItemInSlot = ItemToAdd;
		Slot->Quantity = Quantity;
		ItemsInInventory++;
		Result = true;
		Slot->Update();
	}

	return Result;
}

bool UCharacterInventoryComponent::RemoveItem(FName ItemId)
{
	const FInventorySlot* InventorySlot = GetInventorySlotByItemID(ItemId);
	if (InventorySlot != nullptr)
	{
		InventorySlots.RemoveAll([=](const FInventorySlot& Slot){ return Slot.ItemInSlot->GetItemID() == ItemId; });
		return true;
	}
	return false;
}

int32 UCharacterInventoryComponent::GetInventorySize() const
{
	return Capacity;
}

bool UCharacterInventoryComponent::HasFreeSlot() const
{
	return ItemsInInventory < Capacity;
}

TArray<FInventorySlot> UCharacterInventoryComponent::GetAllItemsCopy() const
{
	return InventorySlots;
}

TArray<FText> UCharacterInventoryComponent::GetAllItemsNames() const
{
	TArray<FText> Result;

	for(const FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.ItemInSlot.IsValid())
		{
			Result.Add(Slot.ItemInSlot->GetItemDescription().Name);
		}
	}
	
	return Result;
}

void UCharacterInventoryComponent::CreateViewWidget(APlayerController* PlayerController)
{
	if (InventoryViewWidget.IsValid())
	{
		return;
	}
	
	if (!IsValid(PlayerController) || !IsValid(InventoryViewWidgetClass))
	{
		return;
	}

	InventoryViewWidget = CreateWidget<UInventoryViewWidget>(PlayerController, InventoryViewWidgetClass);
	InventoryViewWidget->InitializeWidget(InventorySlots);
}

FInventorySlot* UCharacterInventoryComponent::GetInventorySlotByItemID(FName ItemID)
{
	return InventorySlots.FindByPredicate([=](const FInventorySlot& Slot){ return Slot.ItemInSlot->GetItemID() == ItemID; });
}

FInventorySlot* UCharacterInventoryComponent::FindFreeInventorySlot()
{
	return InventorySlots.FindByPredicate([=](const FInventorySlot& Slot){ return !Slot.ItemInSlot.IsValid(); });
}
