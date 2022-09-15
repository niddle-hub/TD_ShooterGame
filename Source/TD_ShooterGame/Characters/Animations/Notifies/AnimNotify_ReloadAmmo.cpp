#include "AnimNotify_ReloadAmmo.h"

#include "TD_ShooterGame/TD_ShooterGameCharacter.h"
#include "TD_ShooterGame/Components/CharacterComponents/CharacterEquipmentComponent.h"

void UAnimNotify_ReloadAmmo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const ATD_ShooterGameCharacter* Character = Cast<ATD_ShooterGameCharacter>(MeshComp->GetOwner());
	if (!IsValid(Character))
	{
		return;
	}
	Character->GetCharacterEquipmentComponent_Mutable()->ReloadAmmoInEquippedWeapon(NumberOfAmmo, true);
}
