#include "Turret.h"

#include "AIController.h"
#include "TD_ShooterGame/Components/ActorComponents/AttributesComponent.h"
#include "TD_ShooterGame/Components/WeaponComponents/ExplosionComponent.h"
#include "TD_ShooterGame/Components/WeaponComponents/WeaponBarrelComponent.h"
#include "TD_ShooterGame/Interfaces/DeathInterface.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	TurretRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TurretRoot"));
	TurretRoot->SetupAttachment(SceneRoot);

	BarrelRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BarrelRoot"));
	BarrelRoot->SetupAttachment(TurretRoot);

	BarrelComponent = CreateDefaultSubobject<UWeaponBarrelComponent>(TEXT("BarrelComponent"));
	BarrelComponent->SetupAttachment(BarrelRoot);

	AttributesComponent = CreateDefaultSubobject<UAttributesComponent>(TEXT("AttributesComponent"));
	ExplosionComponent = CreateDefaultSubobject<UExplosionComponent>(TEXT("ExplosionComponent"));
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	TurretDestroyedDelegateHandle = AttributesComponent->OnDeath.AddUObject(this, &ATurret::OnDestroyed);
}

void ATurret::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AAIController* AIController = Cast<AAIController>(NewController);
	if (IsValid(AIController))
	{
		const FGenericTeamId TeamId(static_cast<uint8>(Team));
		AIController->SetGenericTeamId(TeamId);
	}
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (TurretState)
	{
		case ETurretState::TS_Searching: Searching(DeltaTime); break;
		case ETurretState::TS_Firing: Firing(DeltaTime); break;
		default: break;
	}
}

void ATurret::SetTarget(const TWeakObjectPtr<AActor> NewTarget)
{
	TargetActor = NewTarget;
	const ETurretState NewState = TargetActor.IsValid() ? ETurretState::TS_Firing : ETurretState::TS_Searching;
	SetTurretState(NewState);
}

FVector ATurret::GetPawnViewLocation() const
{
	return BarrelComponent->GetComponentLocation();
}

FRotator ATurret::GetViewRotation() const
{
	return BarrelComponent->GetComponentRotation();
}

void ATurret::Searching(const float DeltaTime) const
{
	FRotator NewRotation = TurretRoot->GetRelativeRotation();
	NewRotation.Yaw += DeltaTime * SearchingRotationRate;
	TurretRoot->SetRelativeRotation(NewRotation);

	FRotator NewBarrelRotation = BarrelRoot->GetRelativeRotation();
	NewBarrelRotation.Pitch = FMath::FInterpTo(NewBarrelRotation.Pitch, 0.f, DeltaTime, BarrelPitchRotationRate);
	BarrelRoot->SetRelativeRotation(NewBarrelRotation);
}

void ATurret::Firing(const float DeltaTime)
{
	if (IDeathInterface* DeathInterface = Cast<IDeathInterface>(TargetActor.Get()))
	{
		if (DeathInterface->IsDead())
		{
			SetTarget(nullptr);
			return;
		}
	}
	
	const FVector BaseLookAtDirection = (TargetActor->GetActorLocation() - TurretRoot->GetComponentLocation()).GetSafeNormal2D();
	const FQuat LookAtQuat = BaseLookAtDirection.ToOrientationQuat();
	const FQuat TargetQuat = FMath::QInterpTo(TurretRoot->GetComponentQuat(), LookAtQuat, DeltaTime, FiringInterpSpeed);
	TurretRoot->SetWorldRotation(TargetQuat);

	const FVector BarrelLookAtDirection = (TargetActor->GetActorLocation() - BarrelRoot->GetComponentLocation()).GetSafeNormal();
	const float BarrelLookAtPitchAngle = BarrelLookAtDirection.ToOrientationRotator().Pitch;

	FRotator BarrelRotation = BarrelRoot->GetRelativeRotation();
	BarrelRotation.Pitch = FMath::FInterpTo(BarrelRotation.Pitch, BarrelLookAtPitchAngle, DeltaTime, BarrelPitchRotationRate);
	BarrelRoot->SetRelativeRotation(BarrelRotation);
}

void ATurret::SetTurretState(const ETurretState NewState)
{
	if (NewState == TurretState)
	{
		return;
	}
	TurretState = NewState;

	switch (TurretState)
	{
		case ETurretState::TS_Searching:
		{
			StopFiring();
			break;
		}
		case ETurretState::TS_Firing:
		{
			GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATurret::Fire, GetFireInterval(), true, FireDelayTime);
			break;
		}
		default: break;
	}
}

float ATurret::GetFireInterval() const
{
	return 60.f / FireRate;
}

void ATurret::Fire() const
{
	const FVector ShotStart = BarrelComponent->GetComponentLocation();
	const FVector ShotDirection = BarrelComponent->GetComponentRotation().RotateVector(FVector::ForwardVector);
	const float SpreadAngle = FMath::DegreesToRadians(BulletSpreadAngle);
	BarrelComponent->Shot(ShotStart, ShotDirection, nullptr, SpreadAngle);
}

void ATurret::StopFiring()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void ATurret::OnDestroyed()
{
	if (!bIsDestroyed)
	{
		bIsDestroyed = true;
		StopFiring();
		SetTurretState(ETurretState::TS_Destroyed);
		ExplosionComponent->Explode(GetController());
		TurretRoot->SetVisibility(false, true);
		AttributesComponent->OnDeath.Remove(TurretDestroyedDelegateHandle);
		SetLifeSpan(3.f); // Destroy after 3 seconds
	}
}
