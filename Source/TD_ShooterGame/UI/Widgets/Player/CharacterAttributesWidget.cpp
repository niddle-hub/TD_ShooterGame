#include "CharacterAttributesWidget.h"

#include "TD_ShooterGame/TD_ShooterGameCharacter.h"
#include "TD_ShooterGame/Components/ActorComponents/AttributesComponent.h"

void UCharacterAttributesWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	APawn* Pawn = GetOwningPlayerPawn();
	ATD_ShooterGameCharacter* Character = Cast<ATD_ShooterGameCharacter>(Pawn);
	if (IsValid(Character))
	{
		CharacterOwner = Character;
	}
}

float UCharacterAttributesWidget::GetHealth() const
{
	float Result = 1.f;
	if (CharacterOwner.IsValid())
	{
		Result = CharacterOwner->GetCharacterAttributesComponent()->GetHealth();
	}

	return Result;
}

float UCharacterAttributesWidget::GetHealthPercent() const
{
	float Result = 1.f;
	if (CharacterOwner.IsValid())
	{
		Result = CharacterOwner->GetCharacterAttributesComponent()->GetHealthPercent();
	}
	return Result;
}

void UCharacterAttributesWidget::UpdateHealth(float NewHealth)
{
	OnHealthChanged(NewHealth);
}