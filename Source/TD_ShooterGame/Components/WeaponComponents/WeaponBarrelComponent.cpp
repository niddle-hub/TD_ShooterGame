#include "WeaponBarrelComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"
#include "TD_ShooterGame/TD_ShooterGameTypes.h"

void UWeaponBarrelComponent::Shot(const FVector ShotStart, FVector ShotDirection, AController* Controller, float SpreadAngle) const
{
	for (int32 i = 0; i < BulletsPerShot; i ++)
	{
		ShotDirection += GetBulletSpreadOffset(FMath::RandRange(0.f, SpreadAngle), ShotDirection.ToOrientationRotator());
	
		const FVector MuzzleLocation = GetComponentLocation();
		FVector ShotEnd = ShotStart + GetCurrentFireRange() * ShotDirection;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireFX, MuzzleLocation, GetComponentRotation());

		FHitResult ShotResult;
	
		if(GetWorld()->LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Bullet))
		{
			ShotEnd = ShotResult.ImpactPoint;
			AActor* HitActor = ShotResult.GetActor();
			if (IsValid(HitActor))
			{
				float DamageAmount = GetCurrentDamage();
				if(IsValid(DamageCurve))
				{
					DamageAmount = GetCurrentDamage() * DamageCurve->GetFloatValue(ShotResult.Distance);
				}
				FPointDamageEvent DamageEvent(DamageAmount, ShotResult, ShotDirection, DamageType);
				HitActor->TakeDamage(DamageAmount, DamageEvent, Controller, GetOwner());
				UAISense_Damage::ReportDamageEvent(GetWorld(), HitActor, GetOwner(), DamageAmount, GetOwner()->GetActorLocation(), ShotResult.ImpactNormal);
			}

			UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DefaultDecal.DecalMaterial, DefaultDecal.DecalSize, ShotResult.ImpactPoint, ShotResult.ImpactNormal.ToOrientationRotator());
			if (IsValid(DecalComponent))
			{
				DecalComponent->SetFadeScreenSize(0.001f);
				DecalComponent->SetFadeOut(DefaultDecal.DecalLifeTime, DefaultDecal.DecalFadeOutTime);
			}
		}
		UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, MuzzleLocation, GetComponentRotation());
		TraceFXComponent->SetVectorParameter(NiagaraProperty::TraceEnd, ShotEnd);
	}
}

FVector UWeaponBarrelComponent::GetBulletSpreadOffset(const float Angle, const FRotator ShotRotation) const
{
	const float SpreadSize = FMath::Tan(Angle);
	const float RotationAngle = FMath::RandRange(0.f, 2 * PI);
	const float SpreadY = FMath::Cos(RotationAngle);
	const float SpreadZ = FMath::Sin(RotationAngle);

	return (ShotRotation.RotateVector(FVector::UpVector) * SpreadZ + ShotRotation.RotateVector(FVector::RightVector) * SpreadY) * SpreadSize;
}

float UWeaponBarrelComponent::GetCurrentFireRange() const
{
	return FireRange;
}

float UWeaponBarrelComponent::GetCurrentDamage() const
{
	return BaseDamage;
}
