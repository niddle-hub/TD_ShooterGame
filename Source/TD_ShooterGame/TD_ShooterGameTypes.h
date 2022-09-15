#pragma once

#define ECC_Bullet ECC_GameTraceChannel1

namespace CollisionProfile
{
	static const FName Ragdoll = "Ragdoll";
}

namespace NiagaraProperty
{
	static const FName TraceEnd = "TraceEnd";
}

namespace Socket
{
	static const FName ForeGrip = "ForeGrip";
	static const FName WeaponMuzzle = "Muzzle";
	static const FName CharacterMuzzle = "weapon_r_muzzle";
	static const FName CharacterWeapon = "weapon_r_socket";
}

namespace MontageSection
{
	static const FName EndReloading = "EndReloading";
}

namespace EventAction
{
	static const FName Interact = "Interact";
}

namespace AssetPath
{
	static const TCHAR* DataTableWeapons = TEXT("/Game/Core/Data/DT_Weapons.DT_Weapons");
	static const TCHAR* DataTableItems = TEXT("/Game/Core/Data/DT_InventoryItems.DT_InventoryItems");
	static const TCHAR* DataTableAmmo = TEXT("/Game/Core/Data/DT_Ammo.DT_Ammo");
}

UENUM(BlueprintType)
enum class EEquipableItemType : uint8
{
	EIT_None UMETA(DisplayName = "None"),
	EIT_Pistol UMETA(DisplayName = "Pistol"),
	EIT_AssaultRifle UMETA(DisplayName = "AssaultRifle"),
	EIT_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EAmmunitionType : uint8
{
	AT_None UMETA(DisplayName = "None"),
	AT_Pistol UMETA(DisplayName = "Pistol"),
	AT_AssaultRifle UMETA(DisplayName = "AssaultRifle"),
	AT_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	ES_None UMETA(DisplayName = "None"),
	ES_PrimaryWeapon UMETA(DisplayName = "PrimaryWeapon"),
	ES_SecondaryWeapon UMETA(DisplayName = "SecondaryWeapon"),
	ES_MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EShooterTeam : uint8
{
	ST_Player UMETA(DisplayName = "Player", ToolTip = "Friendly team"),
	ST_Enemy UMETA(DisplayName = "Enemy", ToolTip = "Enemy team"),
	ST_MAX UMETA(Hidden)
};
