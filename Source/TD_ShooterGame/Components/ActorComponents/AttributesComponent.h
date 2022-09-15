#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TD_ShooterGame/Subsystems/SaveSubsystem/SaveSubsystemInterface.h"
#include "AttributesComponent.generated.h"

class ATD_ShooterGameCharacter;

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TD_SHOOTERGAME_API UAttributesComponent : public UActorComponent, public ISaveSubsystemInterface
{
	GENERATED_BODY()

public:	
	FOnDeathSignature OnDeath;
	FOnHealthChangedSignature OnHealthChanged;

	bool IsAlive() const { return CurrentHealth > 0.f; }
	
	float GetHealth() const { return CurrentHealth; }
	float GetMaxHealth() const { return MaxHealth; }
	float GetHealthPercent() const { return CurrentHealth / MaxHealth; }

	void AddHealth(float HealthAmount);

/** ISaveSubsystemInterface **/
	virtual void OnLevelDeserialized_Implementation() override;
/** ~ISaveSubsystemInterface **/

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes|Health", meta=(UIMin = 0))
	float MaxHealth = 100.f;

private:
	UPROPERTY(SaveGame)
	float CurrentHealth = 0.f;
	
	void SetHealth(float NewHealth);
	
	static bool IsValidDamage(const float Damage) { return Damage > 0.f; }

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser );
};
