#pragma once

#include "CoreMinimal.h"
#include "TD_ShooterGame/Actors/Interactive/Pickables/PickableItem.h"
#include "PickableBooster.generated.h"

UCLASS(Blueprintable)
class TD_SHOOTERGAME_API APickableBooster : public APickableItem
{
	GENERATED_BODY()

public:
	APickableBooster();
	
	virtual void Interact(ATD_ShooterGameCharacter* Caller) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PickableBooster")
	UStaticMeshComponent* PickableMesh;
};
