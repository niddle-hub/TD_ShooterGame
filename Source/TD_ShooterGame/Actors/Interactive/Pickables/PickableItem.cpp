#include "PickableItem.h"

#include "TD_ShooterGame/TD_ShooterGameCharacter.h"

APickableItem::APickableItem()
{
	ItemRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ItemRoot"));
	RootComponent = ItemRoot;
}

void APickableItem::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(InteractionVolume))
	{
		InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &APickableItem::OnInteractionVolumeBeginOverlap);
	}
}

void APickableItem::OnInteractionVolumeBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OnInteractEvent.IsBound())
	{
		OnInteractEvent.Execute();
	}
	
	ATD_ShooterGameCharacter* Player = Cast<ATD_ShooterGameCharacter>(OtherActor);
	if(IsValid(Player))
	{
		Interact(Player);
	}
}
