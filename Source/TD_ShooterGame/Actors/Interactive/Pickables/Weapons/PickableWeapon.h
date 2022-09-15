#pragma once

#include "CoreMinimal.h"
#include "TD_ShooterGame/Actors/Interactive/Pickables/PickableItem.h"
#include "PickableWeapon.generated.h"

UCLASS(Blueprintable)
class TD_SHOOTERGAME_API APickableWeapon : public APickableItem
{
	GENERATED_BODY()

public:
	APickableWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void Interact(ATD_ShooterGameCharacter* Caller) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;
};
