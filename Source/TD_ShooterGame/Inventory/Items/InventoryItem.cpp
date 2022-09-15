#include "InventoryItem.h"

void UInventoryItem::Initialize(FName InItemID, const FInventoryItemDescription& In_ItemDescription)
{
	ItemID = InItemID;
	ItemDescription.Name = In_ItemDescription.Name;
	if (In_ItemDescription.Icon.IsValid())
	{
		ItemDescription.Icon = In_ItemDescription.Icon.LoadSynchronous();
	}
	bIsInitialized = true;
}

bool UInventoryItem::Consume(ATD_ShooterGameCharacter* Character)
{
	if (IsConsumable())
	{
		return InternalConsume(Character);
	}
	return false;
}
