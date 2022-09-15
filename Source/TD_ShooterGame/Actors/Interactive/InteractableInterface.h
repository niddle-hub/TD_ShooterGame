#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

class ATD_ShooterGameCharacter;

UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class TD_SHOOTERGAME_API IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(ATD_ShooterGameCharacter* Caller) PURE_VIRTUAL(IInteractableInterface::Interact, );
};
