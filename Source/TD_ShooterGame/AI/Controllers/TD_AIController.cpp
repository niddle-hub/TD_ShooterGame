#include "TD_AIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "TD_ShooterGame/Interfaces/DeathInterface.h"

ATD_AIController::ATD_AIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

AActor* ATD_AIController::GetClosestSensedActor(TSubclassOf<UAISense> SenseClass) const
{
	if (!IsValid(GetPawn()))
	{
		return nullptr;
	}

	TArray<AActor*> PerceivedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(SenseClass, PerceivedActors);

	AActor* ClosestActor = nullptr;
	float MinSquaredDistance = FLT_MAX;
	const FVector PawnLocation = GetPawn()->GetActorLocation();
	
	for (AActor* PerceivedActor : PerceivedActors)
	{
		IDeathInterface* DeathInterface = Cast<IDeathInterface>(PerceivedActor);
		
		if (PerceivedActor == GetPawn())
		{
			continue;
		}
		if (DeathInterface && DeathInterface->IsDead())
		{
			continue;
		}

		const float SquaredDistance = FVector::DistSquared(PerceivedActor->GetActorLocation(), PawnLocation);
		if (SquaredDistance < MinSquaredDistance)
		{
			MinSquaredDistance = SquaredDistance;
			ClosestActor = PerceivedActor;
		}
	}

	return ClosestActor;
}
