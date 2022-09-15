#include "RangeWeaponItem.h"

#include "TD_ShooterGame/TD_ShooterGameTypes.h"
#include "TD_ShooterGame/Components/WeaponComponents/WeaponBarrelComponent.h"

ARangeWeaponItem::ARangeWeaponItem()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	
	WeaponBarrel = CreateDefaultSubobject<UWeaponBarrelComponent>(TEXT("WeaponBarrel"));
	WeaponBarrel->SetupAttachment(WeaponMesh, Socket::WeaponMuzzle);
}

void ARangeWeaponItem::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	CharacterOwner = StaticCast<ATD_ShooterGameCharacter*>(NewOwner);
}

void ARangeWeaponItem::Hide() const
{
	WeaponMesh->SetVisibility(false);
}

void ARangeWeaponItem::UnHide() const
{
	WeaponMesh->SetVisibility(true);
}

FTransform ARangeWeaponItem::GetForeGripTransform() const
{
	return WeaponMesh->GetSocketTransform(Socket::ForeGrip);
}

void ARangeWeaponItem::OnLevelDeserialized_Implementation()
{
	SetActorRelativeTransform(FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector));
	SetAmmo(CurrentAmmo);
}

void ARangeWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	SetAmmo(MaxAmmo);
	if (!IsEquipped())
	{
		WeaponMesh->SetVisibility(false);
	}
}

float ARangeWeaponItem::GetCurrentBulletSpreadAngle() const
{
	return FMath::DegreesToRadians(SpreadAngle);
}

float ARangeWeaponItem::GetBulletsPerShot() const
{
	switch (WeaponFireMode)
	{
		case EWeaponFireMode::WFM_Burst:
		{
			return BurstShots;
		}
		default: return 1.f;
	}
}

bool ARangeWeaponItem::CheckReloadRequiredForOwner()
{
	if (!CanShoot())
	{
		StopFire();
		if (CurrentAmmo == 0 && bAutoReload)
		{
			CharacterOwner.Get()->ReloadEquippedWeapon();
		}
		return true;
	}
	return false;
}

void ARangeWeaponItem::Shot()
{
	if (CheckReloadRequiredForOwner()) return;
	StopReload(false);
	
	CharacterOwner->PlayAnimMontage(CharacterFireMontage);
	PlayAnimMontage(FireMontage);
	
	APlayerController* PlayerController = CharacterOwner->GetController<APlayerController>();
	if (!IsValid(PlayerController))
	{
		return;
	}

	//TODO: direct shot to mouse cursor hit location
	FHitResult MouseHitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseHitResult);
	const FVector ShotStart = CharacterOwner->GetMesh()->GetSocketLocation(Socket::CharacterMuzzle);
	const FVector ShotDirection = (MouseHitResult.Location - ShotStart) * FVector(1.f, 1.f, 0.f);

	float BulletsPerShot = GetBulletsPerShot();
	for(BulletsPerShot; BulletsPerShot > 0; BulletsPerShot--)
	{
		WeaponBarrel->Shot(ShotStart, ShotDirection, PlayerController, GetCurrentBulletSpreadAngle());
	
		if (!bInfiniteAmmo)
		{
			CurrentAmmo--;
			SetAmmo(CurrentAmmo);
		}

		if (CheckReloadRequiredForOwner()) return;
	}

	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ARangeWeaponItem::OnShotTimerElapsed, GetFireTimerInterval(), false);
	
}

void ARangeWeaponItem::OnShotTimerElapsed()
{
	if (!bIsFiring)
	{
		return;
	}

	switch (WeaponFireMode)
	{
		case EWeaponFireMode::WFM_Single:
		{
			StopFire(); break;
		}
		default:
		{
			Shot();
		}
	}
}

float ARangeWeaponItem::GetFireTimerInterval() const
{
	return 60.f / FireRate;
}

void ARangeWeaponItem::StartFire()
{
	if(GetWorldTimerManager().IsTimerActive(FireTimerHandle))
	{
		return;
	}
	
	bIsFiring = true;
	Shot();
}

void ARangeWeaponItem::StopFire()
{
	bIsFiring = false;
}

void ARangeWeaponItem::StartReload()
{
	if (bIsReloading)
	{
		return;
	}

	bIsReloading = true;
	
	if (IsValid(CharacterReloadMontage))
	{
		const float MontageDuration = CharacterOwner->PlayAnimMontage(CharacterReloadMontage);
		PlayAnimMontage(ReloadMontage);
		if (WeaponReloadFormat == EWeaponReloadFormat::WRF_FullClip)
		{
			GetWorldTimerManager().SetTimer(ReloadingTimerHandle, [&]()
			{
				StopReload(true);
			}, MontageDuration, false);
		}
	}
	else
	{
		StopReload(true);
	}
}

void ARangeWeaponItem::StopReload(bool IsSuccess)
{
	if (!bIsReloading)
	{
		return;
	}

	if (!IsSuccess)
	{
		CharacterOwner->StopAnimMontage(CharacterReloadMontage);
		StopAnimMontage(ReloadMontage);
	}

	if (WeaponReloadFormat == EWeaponReloadFormat::WRF_ByBullet)
	{
		UAnimInstance* CharacterAnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
		if(IsValid(CharacterAnimInstance))
		{
			CharacterAnimInstance->Montage_JumpToSection(MontageSection::EndReloading, CharacterReloadMontage);
		}

		UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
		if (IsValid(WeaponAnimInstance))
		{
			WeaponAnimInstance->Montage_JumpToSection(MontageSection::EndReloading, ReloadMontage);
		}
	}

	bIsReloading = false;

	GetWorldTimerManager().ClearTimer(ReloadingTimerHandle);
	if (IsSuccess && OnReloadCompleteDelegate.IsBound())
	{
		OnReloadCompleteDelegate.Broadcast();
	}
}

FString ARangeWeaponItem::GetAmmoTypeAsString() const
{
	FString AmmoTypeAsStringResult = "None";
	switch (AmmoType)
	{
		case EAmmunitionType::AT_Pistol: AmmoTypeAsStringResult = "Pistol"; break;
		case EAmmunitionType::AT_AssaultRifle: AmmoTypeAsStringResult = "Rifle"; break;
		default: break;
	}
	return AmmoTypeAsStringResult;
}

void ARangeWeaponItem::SetAmmo(const int32 NewAmmo)
{
	CurrentAmmo = NewAmmo;
	if (OnAmmoChangedDelegate.IsBound())
	{
		OnAmmoChangedDelegate.Broadcast(CurrentAmmo);
	}
}

bool ARangeWeaponItem::CanShoot() const
{
	return CurrentAmmo > 0 || bInfiniteAmmo;
}

float ARangeWeaponItem::PlayAnimMontage(UAnimMontage* Montage, float InPlayRate) const
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	float Result = 0.f;
	if (IsValid(WeaponAnimInstance))
	{
		Result = WeaponAnimInstance->Montage_Play(Montage, InPlayRate);
	}
	return Result;
}

void ARangeWeaponItem::StopAnimMontage(const UAnimMontage* Montage, const float BlendOut) const
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	if (IsValid(WeaponAnimInstance))
	{
		WeaponAnimInstance->Montage_Stop(BlendOut, Montage);
	}
}
