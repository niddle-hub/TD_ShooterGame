#pragma once

#include "CoreMinimal.h"
#include "TD_ShooterGame/Inventory/Items/InventoryItem.h"
#include "MedKit.generated.h"

UCLASS()
class TD_SHOOTERGAME_API UMedKit : public UInventoryItem
{
	GENERATED_BODY()

public:
	virtual bool InternalConsume(ATD_ShooterGameCharacter* ConsumeTarget) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MedKit")
	float HealAmount = 25.f;
};
