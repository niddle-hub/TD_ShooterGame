#pragma once

#include "CoreMinimal.h"
#include "TD_ShooterGame/TD_ShooterGameCharacter.h"
#include "TD_ShooterGame/Actors/Equipment/EquipableItem.h"
#include "RangeWeaponItem.generated.h"

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	WFM_Single UMETA(DisplayName = "Single"),
	WFM_Burst UMETA(DisplayName = "Burst"),
	WFM_Auto UMETA(DisplayName = "Auto"),
	WFM_MAX UMETA(Hidden)
};

UENUM()
enum class EWeaponReloadFormat : uint8
{
	WRF_FullClip UMETA(DisplayName = "Full Clip"),
	WRF_ByBullet UMETA(DisplayName = "By Bullet"),
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE(FOnReloadCompleteSignature);

UCLASS(Blueprintable)
class TD_SHOOTERGAME_API ARangeWeaponItem : public AEquipableItem, public ISaveSubsystemInterface
{
	GENERATED_BODY()
	
public:
	ARangeWeaponItem();

	virtual void SetOwner(AActor* NewOwner) override;

	virtual void Hide() const override;
	virtual void UnHide() const override;

	void StartFire();
	void StopFire();

	void StartReload();
	void StopReload(bool IsSuccess);

	bool CheckReloadRequiredForOwner();

	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }
	FORCEINLINE int32 GetMaxAmmo() const { return MaxAmmo; }

	EAmmunitionType GetAmmoType() const { return AmmoType; }
	FString GetAmmoTypeAsString() const;

	void SetAmmo(const int32 NewAmmo);

	bool CanShoot() const;

	bool HasInfiniteAmmo() const { return bInfiniteAmmo; }

	bool IsFullClip() const { return CurrentAmmo == MaxAmmo; }

	FOnAmmoChangedSignature OnAmmoChangedDelegate;
	FOnReloadCompleteSignature OnReloadCompleteDelegate;

	FTransform GetForeGripTransform() const;

/** ISaveSubsystemInterface **/
	virtual void OnLevelDeserialized_Implementation() override;
/** ~ISaveSubsystemInterface **/
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RangeWeapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Animations|Fire")
	UAnimMontage* FireMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Animations|Fire")
	UAnimMontage* CharacterFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Animations|Reloading")
	UAnimMontage* ReloadMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Animations|Reloading")
	UAnimMontage* CharacterReloadMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RangeWeapon|Components")
	class UWeaponBarrelComponent* WeaponBarrel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Properties|Fire")
	EWeaponFireMode WeaponFireMode = EWeaponFireMode::WFM_Single;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Properties|Reloading")
	EWeaponReloadFormat WeaponReloadFormat = EWeaponReloadFormat::WRF_FullClip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Properties", meta=(ToolTip = "Rate in rounds per minute", UIMin = 1.0f))
	float FireRate = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Properties", meta=(ToolTip = "Rate in rounds per minute", UIMin = 1.0f))
	float BurstShots = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Properties", meta=(ToolTip = "Bullet spread half angle in degrees", UIMin = 0.0f, UIMax = 10.0f))
	float SpreadAngle = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Properties|Ammo")
	bool bInfiniteAmmo = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Properties|Reloading")
	bool bAutoReload = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Properties|Ammo")
	EAmmunitionType AmmoType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeWeapon|Properties|Ammo", meta=(UIMin=0))
	int32 MaxAmmo = 30;

private:
	UPROPERTY(SaveGame)
	int32 CurrentAmmo = 0;
	
	float GetCurrentBulletSpreadAngle() const;

	float GetBulletsPerShot() const;

	void Shot();

	void OnShotTimerElapsed();
	
	float GetFireTimerInterval() const;
	
	float PlayAnimMontage(UAnimMontage* Montage, float InPlayRate = 1.f) const;
	void StopAnimMontage(const UAnimMontage* Montage, float BlendOut = 0.f) const;

	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadingTimerHandle;
	
	bool bIsFiring = false;
	bool bIsReloading = false;

	TWeakObjectPtr<ATD_ShooterGameCharacter> CharacterOwner;

};
