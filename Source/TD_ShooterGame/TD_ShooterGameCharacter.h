#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "TD_ShooterGameTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/DeathInterface.h"
#include "Subsystems/SaveSubsystem/SaveSubsystemInterface.h"
#include "TD_ShooterGameCharacter.generated.h"

class UInventoryItem;
class AEquipableItem;
class UCharacterInventoryComponent;
class UCharacterEquipmentComponent;
class UAttributesComponent;

UCLASS(Blueprintable)
class ATD_ShooterGameCharacter : public ACharacter, public IGenericTeamAgentInterface, public IDeathInterface, public ISaveSubsystemInterface
{
	GENERATED_BODY()

public:
	ATD_ShooterGameCharacter();

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE const UCharacterEquipmentComponent* GetCharacterEquipmentComponent() const { return CharacterEquipmentComponent; }

	FORCEINLINE UCharacterEquipmentComponent* GetCharacterEquipmentComponent_Mutable() const { return CharacterEquipmentComponent; }

	FORCEINLINE const UAttributesComponent* GetCharacterAttributesComponent() const { return AttributesComponent; }

	FORCEINLINE UAttributesComponent* GetCharacterAttributesComponent_Mutable() const { return AttributesComponent; }
	
	bool CanFire() const;
	void StartFire() const;
	void StopFire() const;
	
	void NextItem() const;
	void PreviousItem() const;

	void ReloadEquippedWeapon() const;
	
	void EquipPrimaryWeapon() const;
	void EquipSecondaryWeapon() const;

	void ReloadWeapon() const;

	bool PickupItem(TWeakObjectPtr<UInventoryItem> Item) const;
	bool EquipItemFromInventory(TWeakObjectPtr<UInventoryItem> Item) const;
	void UseInventory() const;

/**	IGenericTeamAgentInterface **/
	virtual FGenericTeamId GetGenericTeamId() const override;
/**	~IGenericTeamAgentInterface **/

/** IDeathInterface **/
	virtual bool IsDead() override;
/** ~IDeathInterface **/

/** ISaveSubsystemInterface **/
	virtual void OnLevelDeserialized_Implementation() override;
/** ~ISaveSubsystemInterface **/

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Team")
	EShooterTeam Team = EShooterTeam::ST_Player;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	UAttributesComponent* AttributesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	UCharacterEquipmentComponent* CharacterEquipmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	UCharacterInventoryComponent* CharacterInventoryComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Animations")
	UAnimMontage* OnDeathAnimMontage;
	
	virtual void OnDeath();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	void CloseUI() const;

	bool bIsDead = false;
};

