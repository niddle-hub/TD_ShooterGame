#include "AnimNotify_AttachItem.h"

#include "TD_ShooterGame/TD_ShooterGameCharacter.h"
#include "TD_ShooterGame/Components/CharacterComponents/CharacterEquipmentComponent.h"

void UAnimNotify_AttachItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	const ATD_ShooterGameCharacter* Owner = Cast<ATD_ShooterGameCharacter>(MeshComp->GetOwner());
	if (!IsValid(Owner))
	{
		return;
	}
	Owner->GetCharacterEquipmentComponent_Mutable()->OnNewItemEquippedInSlot();
}
