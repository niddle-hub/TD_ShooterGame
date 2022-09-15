#pragma once

#include "CoreMinimal.h"
#include "TD_AIController.h"
#include "AITurretController.generated.h"

class ATurret;

UCLASS()
class TD_SHOOTERGAME_API AAITurretController : public ATD_AIController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

private:
	TWeakObjectPtr<ATurret> Turret;
	
};
