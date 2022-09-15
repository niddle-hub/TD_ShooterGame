#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ExplosionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplodeSignature);

class UNiagaraSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TD_SHOOTERGAME_API UExplosionComponent : public USceneComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Explode(AController* InstigatorController);

	UPROPERTY(BlueprintAssignable)
	FOnExplodeSignature OnExplodeDelegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	bool bDamageOnExplode = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion|Damage")
	float MaxDamage = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion|Damage")
	float MinDamage = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion|Damage")
	float DamageFallOff = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion|Damage")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion|Radius")
	float InnerRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion|Radius")
	float OuterRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion|VFX")
	TWeakObjectPtr<UNiagaraSystem> ExplosionVFX;

private:
	bool bIsExploding = false;
};
