// Copyright Epic Games, Inc. All Rights Reserved.

#include "TD_ShooterGameGameMode.h"
#include "TD_ShooterGamePlayerController.h"
#include "TD_ShooterGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATD_ShooterGameGameMode::ATD_ShooterGameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATD_ShooterGamePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}