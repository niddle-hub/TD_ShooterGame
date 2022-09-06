#pragma once

UENUM(BlueprintType)
enum class EEquipableItemType : uint8
{
	EIT_None UMETA(DisplayName = "None"),
	EIT_Melee UMETA(DisplayName = "Melee"),
	EIT_Range UMETA(DisplayName = "Range"),
	EIT_Throwable UMETA(DisplayName = "Throwable"),
	EIT_MAX UMETA(Hidden)
};