#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponBarrelComponent.generated.h"

USTRUCT(BlueprintType)
struct FDecalInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector DecalSize = FVector(5.f, 5.f, 5.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DecalLifeTime = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DecalFadeOutTime = 5.f;
};

class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TD_SHOOTERGAME_API UWeaponBarrelComponent : public USceneComponent
{
	GENERATED_BODY()
	
public:
	void Shot(FVector ShotStart, FVector ShotDirection, AController* Controller, float SpreadAngle) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel settings|Fire")
	float FireRange = 10000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel settings|Damage")
	float BaseDamage = 15.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel settings|Damage")
	TSubclassOf<UDamageType> DamageType = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel settings|Fire")
	int32 BulletsPerShot = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel settings|Damage")
	UCurveFloat* DamageCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel settings|VFX")
	UNiagaraSystem* FireFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel settings|VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel settings|VFX")
	FDecalInfo DefaultDecal;

private:
	FVector GetBulletSpreadOffset(const float Angle, const FRotator ShotRotation) const;
	float GetCurrentFireRange() const;
	float GetCurrentDamage() const;
		
};
