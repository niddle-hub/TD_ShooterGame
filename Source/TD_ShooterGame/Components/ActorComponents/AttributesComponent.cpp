#include "AttributesComponent.h"

void UAttributesComponent::AddHealth(float HealthAmount)
{
	SetHealth(CurrentHealth + HealthAmount);
}

void UAttributesComponent::OnLevelDeserialized_Implementation()
{
	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(CurrentHealth);
	}
}

void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHealth(MaxHealth);
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UAttributesComponent::OnTakeAnyDamage);
}

void UAttributesComponent::SetHealth(const float NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(CurrentHealth);
	}
}

void UAttributesComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!IsValidDamage(Damage) || !IsAlive())
	{
		return;
	}

	SetHealth(CurrentHealth - Damage);
	
	if (CurrentHealth <= 0.0f)
	{
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}
	}
}
