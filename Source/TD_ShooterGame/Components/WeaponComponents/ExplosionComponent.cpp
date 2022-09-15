#include "ExplosionComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UExplosionComponent::Explode(AController* InstigatorController)
{
	if (bIsExploding)
	{
		return;
	}
	
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetOwner());

	if (bDamageOnExplode)
	{
		UGameplayStatics::ApplyRadialDamageWithFalloff(
		GetWorld(),
		MaxDamage,
		MinDamage,
		GetComponentLocation(),
		InnerRadius,
		OuterRadius,
		DamageFallOff,
		DamageType,
		IgnoredActors,
		GetOwner(),
		InstigatorController,
		ECC_Visibility);
	}

	if (ExplosionVFX.IsValid())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX.Get(), GetComponentLocation());
	}
	
	if (OnExplodeDelegate.IsBound())
	{
		OnExplodeDelegate.Broadcast();
	}
	
	bIsExploding = true;
}
