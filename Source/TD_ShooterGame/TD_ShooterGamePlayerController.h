// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TD_ShooterGamePlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UPlayerHUDWidget;
class ATD_ShooterGameCharacter;

UCLASS()
class ATD_ShooterGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATD_ShooterGamePlayerController();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void SetPawn(APawn* InPawn) override;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void EquipPrimaryWeapon();
	void EquipSecondaryWeapon();

	void InternalStartFire();
	void InternalStopFire();

	void ReloadWeapon();

	void RotateHitLocation(float DeltaTime);

	void OpenInventory();

	void QuickSaveGame();
	void QuickLoadGame();

private:
	TSoftObjectPtr<ATD_ShooterGameCharacter> CachedCharacter;

	bool bFireInputPressed = false; // Input is bring pressed
	bool bMoveInputPressed = false; // Input is bring pressed
	float MoveFollowTime = 0.f; // For how long it has been pressed
	void CreateWidgets();
	UPlayerHUDWidget* PlayerHUDWidget = nullptr;
};
