#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterAttributesWidget.generated.h"

UCLASS()
class TD_SHOOTERGAME_API UCharacterAttributesWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float NewHealth);
	
	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

private:
	TWeakObjectPtr<class ATD_ShooterGameCharacter> CharacterOwner;

	UFUNCTION()
	void UpdateHealth(float NewHealth);
};
