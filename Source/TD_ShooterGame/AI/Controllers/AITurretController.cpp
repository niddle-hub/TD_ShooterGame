#include "AITurretController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"
#include "TD_ShooterGame/AI/Turret.h"

void AAITurretController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	
	if (IsValid(InPawn))
	{
		checkf(InPawn->IsA<ATurret>(), TEXT("AAITurretController::SetPawn - InPawn is not a ATurret"));
		Turret = StaticCast<ATurret*>(InPawn);
	}
	else
	{
		Turret = nullptr;
	}
}

void AAITurretController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
	
	if (!Turret.IsValid())
	{
		return;
	}
	
	const TWeakObjectPtr<AActor> Sight_ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	const TWeakObjectPtr<AActor> Damage_ClosestActor = GetClosestSensedActor(UAISense_Damage::StaticClass());

	if (Sight_ClosestActor.IsValid())
	{
		Turret->SetTarget(Sight_ClosestActor);
	}
	else if (Damage_ClosestActor.IsValid())
	{
		Turret->SetTarget(Damage_ClosestActor);
	}
	else
	{
		Turret->SetTarget(nullptr);
	}
	
}
