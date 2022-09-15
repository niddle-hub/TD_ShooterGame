#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TD_ShooterGame/TD_ShooterGameTypes.h"
#include "TDCharacterAnimInstance.generated.h"

UCLASS()
class TD_SHOOTERGAME_API UTDCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Weapon")
	EEquipableItemType EquippedItemType = EEquipableItemType::EIT_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Weapon")
	FTransform ForeGripSocketTransform;


private:
	TWeakObjectPtr<class ATD_ShooterGameCharacter> CachedCharacter;
	
};
