#include "MedKit.h"

#include "TD_ShooterGame/TD_ShooterGameCharacter.h"
#include "TD_ShooterGame/Components/ActorComponents/AttributesComponent.h"

bool UMedKit::InternalConsume(ATD_ShooterGameCharacter* ConsumeTarget)
{
	UAttributesComponent* CharacterAttributes = ConsumeTarget->GetCharacterAttributesComponent_Mutable();
	CharacterAttributes->AddHealth(HealAmount);
	this->ConditionalBeginDestroy();
	return true;
}
