#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TD_ShooterGame/TD_ShooterGameTypes.h"
#include "Turret.generated.h"

UENUM(BlueprintType)
enum class ETurretState : uint8
{
	TS_Searching UMETA(DisplayName = "Searching"),
	TS_Firing UMETA(DisplayName = "Firing"),
	TS_Disabled UMETA(DisplayName = "Disabled"),
	TS_Destroyed UMETA(DisplayName = "Destroyed"),
	TS_MAX UMETA(Hidden)
};

UCLASS()
class TD_SHOOTERGAME_API ATurret : public APawn
{
	GENERATED_BODY()

public:
	ATurret();
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;

	void SetTarget(TWeakObjectPtr<AActor> NewTarget);
	virtual FVector GetPawnViewLocation() const override;
	virtual FRotator GetViewRotation() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Team")
	EShooterTeam Team = EShooterTeam::ST_Enemy;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Components")
	USceneComponent* TurretRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Components")
	USceneComponent* BarrelRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Components")
	class UWeaponBarrelComponent* BarrelComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Components")
	class UAttributesComponent* AttributesComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Components")
	class UExplosionComponent* ExplosionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Vision", meta=(ClampMin = 0.f, UIMin = 0.f))
	float SearchingRotationRate = 60.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Vision", meta=(ClampMin = 0.f, UIMin = 0.f))
	float FiringInterpSpeed = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Vision", meta=(ClampMin = 0.f, UIMin = 0.f))
	float BarrelPitchRotationRate = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Vision", meta=(ClampMin = -180.f, ClampMax = 180.f))
	float MinBarrelPitchAngle = -30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Vision", meta=(ClampMin = -180.f, ClampMax = 180.f))
	float MaxBarrelPitchAngle = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Firing", meta=(ClampMin = 1.f, UIMin = 1.f))
	float FireRate = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Firing", meta=(ClampMin = 0.f, UIMin = 0.f))
	float BulletSpreadAngle = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Firing", meta=(ClampMin = 0.f, UIMin = 0.f))
	float FireDelayTime = 3.f;

private:
	void Searching(float DeltaTime) const;
	void Firing(float DeltaTime);
	
	void SetTurretState(ETurretState NewState);
	ETurretState TurretState = ETurretState::TS_Searching;
	FTimerHandle FireTimerHandle;
	
	float GetFireInterval() const;
	void Fire() const;
	void StopFiring();

	TWeakObjectPtr<AActor> TargetActor;
	FDelegateHandle TurretDestroyedDelegateHandle;

	void OnDestroyed();
	bool bIsDestroyed = false;
};
	
