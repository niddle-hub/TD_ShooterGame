#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TD_ShooterGame/TD_ShooterGameTypes.h"
#include "TD_ShooterGame/Actors/Interactive/Pickables/PickableItem.h"
#include "PickableAmmunition.generated.h"

USTRUCT()
struct FAmmunitionTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunition")
	EAmmunitionType AmmunitionType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunition")
	TSoftObjectPtr<UStaticMesh> AmmoMesh;
};

UCLASS(Blueprintable)
class TD_SHOOTERGAME_API APickableAmmunition : public APickableItem
{
	GENERATED_BODY()

public:
	APickableAmmunition();

	virtual void BeginPlay() override;
	virtual void Interact(ATD_ShooterGameCharacter* Caller) override;

	EAmmunitionType GetAmmunitionType() const { return AmmunitionType; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammunition")
	UStaticMeshComponent* AmmoMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunition")
	int32 AmmunitionAmount;

private:
	TSoftObjectPtr<UStaticMesh> LoadedMesh;
	EAmmunitionType AmmunitionType = EAmmunitionType::AT_None;
	
};
