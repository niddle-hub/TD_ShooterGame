#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TD_AIController.generated.h"

class UAISense;

UCLASS()
class TD_SHOOTERGAME_API ATD_AIController : public AAIController
{
	GENERATED_BODY()

public:
	ATD_AIController();

protected:
	AActor* GetClosestSensedActor(TSubclassOf<UAISense> SenseClass) const;
	
};
