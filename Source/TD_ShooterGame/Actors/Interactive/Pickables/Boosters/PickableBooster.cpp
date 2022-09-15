#include "PickableBooster.h"

#include "Components/SphereComponent.h"
#include "TD_ShooterGame/TD_ShooterGameCharacter.h"
#include "TD_ShooterGame/TD_ShooterGameTypes.h"
#include "TD_ShooterGame/Inventory/Items/InventoryItem.h"
#include "TD_ShooterGame/Utils/SGDataTableUtils.h"

APickableBooster::APickableBooster()
{
	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	
	PickableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	PickableMesh->SetupAttachment(RootComponent);
	PickableMesh->SetCollisionProfileName("NoCollision");
	PickableMesh->SetGenerateOverlapEvents(false);
}

void APickableBooster::Interact(ATD_ShooterGameCharacter* Caller)
{
	const FItemTableRow* ItemRow = SGDataTableUtils::FindData<FItemTableRow>(GetDataTableID(), AssetPath::DataTableItems);
	if (ItemRow == nullptr)
	{
		return;
	}

	const TWeakObjectPtr<UInventoryItem> Item = TWeakObjectPtr<UInventoryItem>(NewObject<UInventoryItem>(Caller, ItemRow->InventoryItemClass));
	Item->Initialize(DataTableID, ItemRow->InventoryItemDescription);
	const bool bPickedUp = Caller->PickupItem(Item);
	if (bPickedUp)
	{
		Destroy();
	}
}