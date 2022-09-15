#include "TDCharacterAnimInstance.h"

#include "TD_ShooterGame/TD_ShooterGameCharacter.h"
#include "TD_ShooterGame/Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "TD_ShooterGame/Components/CharacterComponents/CharacterEquipmentComponent.h"

void UTDCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<ATD_ShooterGameCharacter>(), TEXT("UTDCharacterAnimInstance can work only with ATD_ShooterGameCharacter instance class"));
	CachedCharacter = StaticCast<ATD_ShooterGameCharacter*>(TryGetPawnOwner());
}

void UTDCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedCharacter.IsValid())
	{
		return;
	}
	
	const UCharacterEquipmentComponent* EquipmentComponent = CachedCharacter->GetCharacterEquipmentComponent();
	EquippedItemType = EquipmentComponent->GetEquippedItemType();

	const ARangeWeaponItem* RangeWeaponItem = EquipmentComponent->GetEquippedRangeWeapon();
	if (IsValid(RangeWeaponItem))
	{
		ForeGripSocketTransform = RangeWeaponItem->GetForeGripTransform();
	}
}
