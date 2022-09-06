#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TD_ShooterGameCharacter.generated.h"

UCLASS(Blueprintable)
class ATD_ShooterGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATD_ShooterGameCharacter();

	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
};

