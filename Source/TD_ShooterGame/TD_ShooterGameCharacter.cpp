#include "TD_ShooterGameCharacter.h"

#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "Components/ActorComponents/AttributesComponent.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "Components/CharacterComponents/CharacterInventoryComponent.h"
#include "Inventory/Items/InventoryItem.h"
#include "Inventory/Items/Weapons/WeaponInventoryItem.h"

ATD_ShooterGameCharacter::ATD_ShooterGameCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AttributesComponent = CreateDefaultSubobject<UAttributesComponent>(TEXT("Character Attributes"));
	CharacterEquipmentComponent = CreateDefaultSubobject<UCharacterEquipmentComponent>(TEXT("Character Equipment"));
	CharacterInventoryComponent = CreateDefaultSubobject<UCharacterInventoryComponent>(TEXT("Character Inventory"));
}

void ATD_ShooterGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AttributesComponent->OnDeath.AddUObject(this, &ATD_ShooterGameCharacter::OnDeath);
}

void ATD_ShooterGameCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AAIController* AIController = Cast<AAIController>(NewController);
	if (IsValid(AIController))
	{
		const FGenericTeamId TeamId(static_cast<uint8>(Team));
		AIController->SetGenericTeamId(TeamId);
	}
}

bool ATD_ShooterGameCharacter::CanFire() const
{
	const bool IsEquipping = CharacterEquipmentComponent->IsEquipping();

	return !IsEquipping && !bIsDead;
}

void ATD_ShooterGameCharacter::StartFire() const
{
	if (!CanFire())
	{
		return;
	}
	ARangeWeaponItem* RangeWeaponItem = CharacterEquipmentComponent->GetEquippedRangeWeapon();
	if (IsValid(RangeWeaponItem))
	{
		RangeWeaponItem->StartFire();
	}
}

void ATD_ShooterGameCharacter::StopFire() const
{
	ARangeWeaponItem* RangeWeaponItem = CharacterEquipmentComponent->GetEquippedRangeWeapon();
	if (IsValid(RangeWeaponItem))
	{
		RangeWeaponItem->StopFire();
	}
}

void ATD_ShooterGameCharacter::NextItem() const
{
	CharacterEquipmentComponent->EquipNext();
}

void ATD_ShooterGameCharacter::PreviousItem() const
{
	CharacterEquipmentComponent->EquipPrevious();
}

void ATD_ShooterGameCharacter::ReloadEquippedWeapon() const
{
	if(IsValid(GetCharacterEquipmentComponent()->GetEquippedRangeWeapon()))
	{
		CharacterEquipmentComponent->ReloadEquippedWeapon();
	}
}

void ATD_ShooterGameCharacter::EquipPrimaryWeapon() const
{
	if (!CharacterEquipmentComponent->IsSlotEmpty(EEquipmentSlot::ES_PrimaryWeapon))
	{
		CharacterEquipmentComponent->EquipItemInSlot(EEquipmentSlot::ES_PrimaryWeapon);
	}
}

void ATD_ShooterGameCharacter::EquipSecondaryWeapon() const
{
	if (!CharacterEquipmentComponent->IsSlotEmpty(EEquipmentSlot::ES_SecondaryWeapon))
	{
		CharacterEquipmentComponent->EquipItemInSlot(EEquipmentSlot::ES_SecondaryWeapon);
	}
}

void ATD_ShooterGameCharacter::ReloadWeapon() const
{
	CharacterEquipmentComponent->ReloadEquippedWeapon();
}

bool ATD_ShooterGameCharacter::PickupItem(TWeakObjectPtr<UInventoryItem> Item) const
{
	if (CharacterEquipmentComponent->IsEnabledAutoEquip())
	{
		if (EquipItemFromInventory(Item))
		{
			return true;
		}
	}
	
	if (CharacterInventoryComponent->HasFreeSlot())
	{
		return CharacterInventoryComponent->AddItem(Item);
	}
	return false;
}

bool ATD_ShooterGameCharacter::EquipItemFromInventory(TWeakObjectPtr<UInventoryItem> Item) const
{
	const UWeaponInventoryItem* WeaponItem = Cast<UWeaponInventoryItem>(Item.Get());
	if (IsValid(WeaponItem))
	{
		const EEquipmentSlot PrioritySlot = WeaponItem->GetEquipWeaponClass().GetDefaultObject()->GetPrioritySlot();
		return CharacterEquipmentComponent->AddEquipmentItemInSlot(WeaponItem->GetEquipWeaponClass(), PrioritySlot);
	}
	return false;
}

void ATD_ShooterGameCharacter::UseInventory() const
{
	if (!IsPlayerControlled())
	{
		return;
	}

	APlayerController* PlayerController = GetController<APlayerController>();
	if(!CharacterInventoryComponent->IsInventoryOpen())
	{
		CharacterInventoryComponent->OpenInventory(PlayerController);
		CharacterEquipmentComponent->OpenEquipmentMenu(PlayerController);
	}
	else
	{
		CharacterInventoryComponent->CloseInventory();
		CharacterEquipmentComponent->CloseEquipmentMenu();
		PlayerController->bShowMouseCursor = true;
	}
}

FGenericTeamId ATD_ShooterGameCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(static_cast<uint8>(Team));
}

bool ATD_ShooterGameCharacter::IsDead()
{
	return bIsDead;
}

void ATD_ShooterGameCharacter::OnLevelDeserialized_Implementation()
{
}

void ATD_ShooterGameCharacter::OnDeath()
{
	if (!bIsDead)
	{
		bIsDead = true;
		APlayerController* PlayerController = GetController<APlayerController>();
		PlayerController->StopMovement();
		GetCharacterMovement()->DisableMovement();
		StopFire();
		GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
		CloseUI();
		PlayAnimMontage(OnDeathAnimMontage);
	}
}

void ATD_ShooterGameCharacter::CloseUI() const
{
	if (CharacterInventoryComponent->IsInventoryOpen())
	{
		CharacterInventoryComponent->CloseInventory();
	}

	if (CharacterEquipmentComponent->IsEquipmentMenuOpened())
	{
		CharacterEquipmentComponent->CloseEquipmentMenu();
	}
}

