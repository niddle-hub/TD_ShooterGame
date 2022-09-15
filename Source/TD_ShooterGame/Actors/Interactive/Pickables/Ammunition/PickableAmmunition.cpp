#include "PickableAmmunition.h"

#include "Components/SphereComponent.h"
#include "TD_ShooterGame/TD_ShooterGameCharacter.h"
#include "TD_ShooterGame/Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "TD_ShooterGame/Utils/SGDataTableUtils.h"

APickableAmmunition::APickableAmmunition()
{
	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	
	AmmoMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	AmmoMeshComp->SetupAttachment(RootComponent);
	AmmoMeshComp->SetCollisionProfileName("NoCollision");
	AmmoMeshComp->SetGenerateOverlapEvents(false);
}

void APickableAmmunition::BeginPlay()
{
	Super::BeginPlay();
	const FAmmunitionTableRow* AmmoRow = SGDataTableUtils::FindData<FAmmunitionTableRow>(GetDataTableID(), AssetPath::DataTableAmmo);
	
	if (AmmoRow == nullptr)
	{
		return;
	}

	if (UStaticMesh* AmmoMesh = AmmoRow->AmmoMesh; IsValid(AmmoMesh))
	{
		AmmoMeshComp->SetStaticMesh(AmmoMesh);
	}
	
	AmmunitionType = AmmoRow->AmmunitionType;
}

void APickableAmmunition::Interact(ATD_ShooterGameCharacter* Caller)
{
	Caller->GetCharacterEquipmentComponent_Mutable()->AddAmmunition(AmmunitionType, AmmunitionAmount);
	Destroy();
}
