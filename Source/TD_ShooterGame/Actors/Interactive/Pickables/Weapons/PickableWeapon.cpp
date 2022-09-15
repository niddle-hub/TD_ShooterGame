#include "PickableWeapon.h"

#include "Components/SphereComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "TD_ShooterGame/TD_ShooterGameCharacter.h"
#include "TD_ShooterGame/TD_ShooterGameTypes.h"
#include "TD_ShooterGame/Inventory/Items/InventoryItem.h"
#include "TD_ShooterGame/Inventory/Items/Weapons/WeaponInventoryItem.h"
#include "TD_ShooterGame/Utils/SGDataTableUtils.h"

APickableWeapon::APickableWeapon()
{
	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetCollisionProfileName("NoCollision");
	WeaponMesh->SetGenerateOverlapEvents(false);
}

void APickableWeapon::Interact(ATD_ShooterGameCharacter* Caller)
{
	if (const FWeaponTableRow* WeaponRow = SGDataTableUtils::FindData<FWeaponTableRow>(DataTableID, AssetPath::DataTableWeapons))
	{
		const TWeakObjectPtr<UWeaponInventoryItem> Weapon = NewObject<UWeaponInventoryItem>(Caller);
		Weapon->Initialize(DataTableID, WeaponRow->WeaponDescription);
		Weapon->SetEquipWeaponClass(WeaponRow->EquipableActorClass);
		if (Caller->PickupItem(Weapon))
		{
			Destroy();
		}
	}
}
