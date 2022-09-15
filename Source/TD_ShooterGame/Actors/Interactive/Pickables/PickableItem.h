#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TD_ShooterGame/Actors/Interactive/InteractableInterface.h"
#include "PickableItem.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnInteractSignature);

UCLASS(Abstract, NotBlueprintable)
class TD_SHOOTERGAME_API APickableItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	APickableItem();
	
	virtual void BeginPlay() override;
	FORCEINLINE const FName& GetDataTableID() const { return DataTableID; }

	UPROPERTY(BlueprintReadWrite)
	FOnInteractSignature OnInteractEvent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FName DataTableID = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* ItemRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPrimitiveComponent* InteractionVolume;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

};
