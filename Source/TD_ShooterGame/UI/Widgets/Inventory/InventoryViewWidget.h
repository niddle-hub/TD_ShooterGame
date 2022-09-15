#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryViewWidget.generated.h"

class UGridPanel;
struct FInventorySlot;
class UInventorySlotWidget;

UCLASS()
class TD_SHOOTERGAME_API UInventoryViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeWidget(TArray<FInventorySlot>& InventorySlots);

protected:
	UPROPERTY(meta=(BindWidget))
	UGridPanel* InventoryGridPanel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory widget")
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory widget")
	int32 ColumnsCount = 4;

	void AddInventorySlotWidget(FInventorySlot& NewSlot);
};
