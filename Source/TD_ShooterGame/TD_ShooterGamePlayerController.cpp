// Copyright Epic Games, Inc. All Rights Reserved.
// ReSharper disable CppMemberFunctionMayBeConst

#include "TD_ShooterGamePlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TD_ShooterGameCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/ActorComponents/AttributesComponent.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/SaveSubsystem/SaveSubsystem.h"
#include "UI/Widgets/Player/CharacterAttributesWidget.h"
#include "UI/Widgets/Player/PlayerHUDWidget.h"
#include "UI/Widgets/Weapon/WeaponAmmoWidget.h"

ATD_ShooterGamePlayerController::ATD_ShooterGamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ATD_ShooterGamePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bFireInputPressed)
	{
		RotateHitLocation(DeltaSeconds);
	}
}

void ATD_ShooterGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	CreateWidgets();
	FInputModeGameAndUI GameAndUIMode;
	GameAndUIMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	GameAndUIMode.SetHideCursorDuringCapture(false);
	SetInputMode(GameAndUIMode);
}

void ATD_ShooterGamePlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn == nullptr)
	{
		return;
	}
	CachedCharacter = Cast<ATD_ShooterGameCharacter>(InPawn);
}

void ATD_ShooterGamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bMoveInputPressed)
	{
		MoveFollowTime += DeltaTime;

		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		const FVector HitLocation = Hit.Location;

		// Direct the Pawn towards that location
		APawn* const MyPawn = GetPawn();
		if (MyPawn)
		{
			const FVector WorldDirection = (HitLocation - MyPawn->GetActorLocation()).GetSafeNormal();
			MyPawn->AddMovementInput(WorldDirection, 1.f, false);
		}
	}
	else
	{
		MoveFollowTime = 0.f;
	}
}

void ATD_ShooterGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATD_ShooterGamePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ATD_ShooterGamePlayerController::OnSetDestinationReleased);
	InputComponent->BindAction("EquipPrimaryWeapon", IE_Pressed, this, &ATD_ShooterGamePlayerController::EquipPrimaryWeapon);
	InputComponent->BindAction("EquipSecondaryWeapon", IE_Pressed, this, &ATD_ShooterGamePlayerController::EquipSecondaryWeapon);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATD_ShooterGamePlayerController::InternalStartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ATD_ShooterGamePlayerController::InternalStopFire);
	InputComponent->BindAction("Reload", IE_Pressed, this, &ATD_ShooterGamePlayerController::ReloadWeapon);
	InputComponent->BindAction("OpenInventory", IE_Pressed, this, &ATD_ShooterGamePlayerController::OpenInventory);
	InputComponent->BindAction("QuickSaveGame", IE_Pressed, this, &ATD_ShooterGamePlayerController::QuickSaveGame);
	InputComponent->BindAction("QuickLoadGame", IE_Pressed, this, &ATD_ShooterGamePlayerController::QuickLoadGame);
}

void ATD_ShooterGamePlayerController::OnSetDestinationPressed()
{
	// We flag that the input is being pressed
	bMoveInputPressed = true;
	// Just in case the character was moving because of a previous short press we stop it
	StopMovement();
}

void ATD_ShooterGamePlayerController::OnSetDestinationReleased()
{
	// Player is no longer pressing the input
	bMoveInputPressed = false;

	// If it was a short press
	if (MoveFollowTime <= ShortPressThreshold)
	{
		InternalStopFire();

		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		const FVector HitLocation = Hit.Location;

		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitLocation);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}

void ATD_ShooterGamePlayerController::EquipPrimaryWeapon()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->EquipPrimaryWeapon();
	}
}

void ATD_ShooterGamePlayerController::EquipSecondaryWeapon()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->EquipSecondaryWeapon();
	}
}

void ATD_ShooterGamePlayerController::InternalStartFire()
{
	bFireInputPressed = true;
	StopMovement();
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->StartFire();
	}
}

void ATD_ShooterGamePlayerController::InternalStopFire()
{
	bFireInputPressed = false;
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->StopFire();
	}
}

void ATD_ShooterGamePlayerController::ReloadWeapon()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->ReloadWeapon();
	}
}

void ATD_ShooterGamePlayerController::OpenInventory()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->UseInventory();
	}
}

void ATD_ShooterGamePlayerController::QuickSaveGame()
{
	USaveSubsystem* SaveSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USaveSubsystem>();
	SaveSubsystem->SaveGame();
}

void ATD_ShooterGamePlayerController::QuickLoadGame()
{
	USaveSubsystem* SaveSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USaveSubsystem>();
	SaveSubsystem->LoadLastGame();
}


void ATD_ShooterGamePlayerController::CreateWidgets()
{
	if (!IsValid(PlayerHUDWidgetClass))
	{
		return;
	}
	
	if (!IsValid(PlayerHUDWidget))
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (IsValid(PlayerHUDWidget))
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	if (CachedCharacter.IsValid() && IsValid(PlayerHUDWidget))
	{
		UWeaponAmmoWidget* WeaponAmmoWidget = PlayerHUDWidget->GetWeaponAmmoWidget();
		if (IsValid(WeaponAmmoWidget))
		{
			UCharacterEquipmentComponent* EquipmentComponent = CachedCharacter->GetCharacterEquipmentComponent_Mutable();
			EquipmentComponent->OnEquippedWeaponAmmoChangedDelegate.AddUFunction(WeaponAmmoWidget, FName("UpdateAmmo"));
		}

		UCharacterAttributesWidget* CharacterAttributesWidget = PlayerHUDWidget->GetCharacterAttributesWidget();
		if (IsValid(CharacterAttributesWidget))
		{
			UAttributesComponent* CharacterAttributesComponent = CachedCharacter->GetCharacterAttributesComponent_Mutable();
			CharacterAttributesComponent->OnHealthChanged.AddUFunction(CharacterAttributesWidget, FName("UpdateHealth"));
		}
	}
}

void ATD_ShooterGamePlayerController::RotateHitLocation(float DeltaTime)
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	const FVector HitLocation = Hit.Location;
	const FVector PawnLocation = GetPawn()->GetActorLocation();
	const FRotator PawnRotation = GetPawn()->GetActorRotation();
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PawnLocation, HitLocation);
	FRotator TargetRotation = FMath::RInterpTo(PawnRotation, LookAtRotation, DeltaTime, 30.f);
	GetPawn()->SetActorRotation(FRotator(0.f, TargetRotation.Yaw, 0.f));
}
