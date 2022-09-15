#include "CharacterEquipmentComponent.h"

#include "TD_ShooterGame/Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "TD_ShooterGame/UI/Widgets/Equipment/EquipmentViewWidget.h"

int32 UCharacterEquipmentComponent::GetMaxAmmunitionAmountForType(EAmmunitionType AmmoType) const
{
	if (AmmoType == EAmmunitionType::AT_None)
	{
		return 0;
	}
	return MaxAmmunitionAmount.FindRef(AmmoType);
}

void UCharacterEquipmentComponent::OnLevelDeserialized_Implementation()
{
	EquipItemInSlot(EquippedSlot);
}

void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<ATD_ShooterGameCharacter>(), TEXT("CharacterEquipmentComponent can be used only with a ATD_ShooterGameCharacter"));
	OwnerCharacter = StaticCast<ATD_ShooterGameCharacter*>(GetOwner());
	OnWeaponChangedDelegate.AddUObject(this, &UCharacterEquipmentComponent::OnWeaponChanged);
	CreateLoadout();
}

EEquipableItemType UCharacterEquipmentComponent::GetEquippedItemType() const
{
	EEquipableItemType Result = EEquipableItemType::EIT_None;
	if (EquippedWeapon.IsValid())
	{
		Result = EquippedWeapon->GetItemType();
	}

	return Result;
}

void UCharacterEquipmentComponent::ReloadEquippedWeapon() const
{
	if (!EquippedWeapon.IsValid())
	{
		return;
	}

	// ReSharper disable once CppTooWideScope
	const bool ShouldSkipReload = EquippedWeapon->HasInfiniteAmmo() || EquippedWeapon->IsFullClip();
	if (ShouldSkipReload)
	{
		return;
	}
	
	const int32 AvailableAmmunition = GetAvailableAmmunitionForEquippedWeapon();
	if (AvailableAmmunition <= 0)
	{
		return;
	}

	EquippedWeapon->StartReload();
}

void UCharacterEquipmentComponent::UnequipCurrentItem()
{
	if (EquippedItem.IsValid())
	{
		EquippedItem->Unequip();
		EquippedItem = nullptr;
	}
	if (EquippedWeapon.IsValid())
	{
		EquippedWeapon->StopFire();
		EquippedWeapon->StopReload(false);
		EquippedWeapon->OnAmmoChangedDelegate.Remove(OnEquippedWeaponAmmoChangedHandle);
		EquippedWeapon->OnReloadCompleteDelegate.Remove(OnEquippedWeaponReloadedHandle);
		EquippedWeapon = nullptr;
	}
	PreviousEquippedSlot = EquippedSlot;
	EquippedSlot = EEquipmentSlot::ES_None;
}

void UCharacterEquipmentComponent::OnNewItemEquippedInSlot() const
{
	if (EquippedItem.IsValid())
	{
		EquippedItem->UnHide();
		EquippedItem->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Socket::CharacterWeapon);
	}

	if (EquippedWeapon.IsValid())
	{
		OnEquippedWeaponAmmoChanged(EquippedWeapon->GetCurrentAmmo());
		BroadcastOnWeaponChanged();
	}
}

bool UCharacterEquipmentComponent::EquipItemInSlot(EEquipmentSlot Slot)
{	
	if (bIsEquipping)
	{
		return false;
	}

	if (EquippedSlot == Slot)
	{
		return false;
	}
	
	UnequipCurrentItem();
	
	EquippedItem = GetItemFromSlot(Slot);
	EquippedWeapon = Cast<ARangeWeaponItem>(EquippedItem);

	if (EquippedWeapon.IsValid())
	{
		OnEquippedWeaponAmmoChangedHandle = EquippedWeapon->OnAmmoChangedDelegate.AddUObject(this, &UCharacterEquipmentComponent::OnEquippedWeaponAmmoChanged);
		OnEquippedWeaponReloadedHandle = EquippedWeapon->OnReloadCompleteDelegate.AddUObject(this, &UCharacterEquipmentComponent::OnEquippedWeaponReloaded);
	}

	if (EquippedItem.IsValid())
	{
		UAnimMontage* EquipMontage = EquippedItem->GetCharacterEquipAnimation();
		if (IsValid(EquipMontage))
		{
			bIsEquipping = true;
			const float EquipDuration = OwnerCharacter->PlayAnimMontage(EquipMontage);
			GetWorld()->GetTimerManager().SetTimer(EquipTimerHandle, [&]()
			{
				bIsEquipping = false;
			}, EquipDuration, false);
		}
		else
		{
			OnNewItemEquippedInSlot();
		}
		EquippedItem->Equip();
	}
	EquippedSlot = Slot;
	return true;
}

AEquipableItem* UCharacterEquipmentComponent::GetItemFromSlot(const EEquipmentSlot Slot) const
{
	return ItemsArray[static_cast<uint32>(Slot)];
}

AEquipableItem* UCharacterEquipmentComponent::GetItemFromSlot(const int32 SlotIndex) const
{
	return ItemsArray[SlotIndex];
}

bool UCharacterEquipmentComponent::IsSlotEmpty(const EEquipmentSlot Slot) const
{
	return !IsValid(GetItemFromSlot(Slot));
}

bool UCharacterEquipmentComponent::IsSlotEmpty(const int32 SlotIndex) const
{
	return !IsValid(GetItemFromSlot(SlotIndex));
}

void UCharacterEquipmentComponent::AddAmmunition(EAmmunitionType AmmoType, const int32 NumberOfAmmo)
{
	const int32 AmmoSlotIndex = static_cast<int32>(AmmoType);
	const int32 CurrentAmmo = AmmunitionArray[AmmoSlotIndex];
	const int32 MaxAmmoToGive = GetMaxAmmunitionAmountForType(AmmoType) - CurrentAmmo;
	const int32 AmmunitionToAdd = FMath::Clamp(NumberOfAmmo, 0, MaxAmmoToGive);
	AmmunitionArray[AmmoSlotIndex] += AmmunitionToAdd;

	if (EquippedWeapon.IsValid())
	{
		OnEquippedWeaponAmmoChanged(EquippedWeapon->GetCurrentAmmo());
		EquippedWeapon->CheckReloadRequiredForOwner();
	}
}

void UCharacterEquipmentComponent::EquipNext()
{	
	const uint32 CurrentSlotIndex = static_cast<uint32>(EquippedSlot);
	uint32 NextSlotIndex = NextItemsArraySlotIndex(CurrentSlotIndex);
	
	while (CurrentSlotIndex != NextSlotIndex && (IgnoreSlotsWhileSwitching.Contains(static_cast<EEquipmentSlot>(NextSlotIndex)) || !IsValid(ItemsArray[NextSlotIndex])))
	{
		NextSlotIndex = NextItemsArraySlotIndex(NextSlotIndex);
	}
	if (CurrentSlotIndex != NextSlotIndex)
	{
		EquipItemInSlot(static_cast<EEquipmentSlot>(NextSlotIndex));
	}
}

void UCharacterEquipmentComponent::EquipPrevious()
{
	const uint32 CurrentSlotIndex = static_cast<uint32>(EquippedSlot);
	uint32 PrevSlotIndex = PreviousItemsArraySlotIndex(CurrentSlotIndex);

	while (CurrentSlotIndex != PrevSlotIndex && ( IgnoreSlotsWhileSwitching.Contains(static_cast<EEquipmentSlot>(PrevSlotIndex)) || !IsValid(ItemsArray[PrevSlotIndex])))
	{
		PrevSlotIndex = PreviousItemsArraySlotIndex(PrevSlotIndex);
	}
	if (CurrentSlotIndex != PrevSlotIndex)
	{
		EquipItemInSlot(static_cast<EEquipmentSlot>(PrevSlotIndex));
	}
}

bool UCharacterEquipmentComponent::AddEquipmentItemInSlot(const TSubclassOf<AEquipableItem> ItemClass, EEquipmentSlot Slot)
{
	if (!IsValid(ItemClass))
	{
		return false;
	}
	const int32 SlotIndex = static_cast<int32>(Slot);
	return AddEquipmentItemInSlot(ItemClass, SlotIndex);
}

bool UCharacterEquipmentComponent::AddEquipmentItemInSlot(const TSubclassOf<AEquipableItem> ItemClass, int32 SlotIndex)
{
	if (!IsValid(ItemClass))
	{
		return false;
	}
	
	const AEquipableItem* EquipableItemObject = ItemClass->GetDefaultObject<AEquipableItem>();
	if (!EquipableItemObject->IsSlotCompatible(static_cast<EEquipmentSlot>(SlotIndex)))
	{
		return false;
	}

	if (IsSlotEmpty(SlotIndex))
	{
		AEquipableItem* Item = GetWorld()->SpawnActor<AEquipableItem>(ItemClass);
		Item->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
		Item->SetOwner(OwnerCharacter.Get());
		Item->Unequip();
		ItemsArray[SlotIndex] = Item;
	}
	else
	{
		return false;
	}
	
	if (ViewWidget.IsValid())
	{
		ViewWidget->UpdateSlot(SlotIndex);
	}
	
	return true;
}

void UCharacterEquipmentComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	AEquipableItem* Item = GetItemFromSlot(SlotIndex);
	if (IsValid(Item))
	{
		if (static_cast<int32>(EquippedSlot) == SlotIndex)
		{
			UnequipCurrentItem();
		}
		Item->Destroy();
		Item = nullptr;
	}

	UpdateAmmo();
}

void UCharacterEquipmentComponent::OpenEquipmentMenu(APlayerController* PlayerController)
{
	if (!ViewWidget.IsValid())
	{
		CreateEquipmentWidget(PlayerController);
	}

	if (!ViewWidget->IsVisible())
	{
		ViewWidget->AddToViewport();
	}
}

void UCharacterEquipmentComponent::CloseEquipmentMenu() const
{
	if (ViewWidget->IsVisible())
	{
		ViewWidget->RemoveFromParent();
	}
}

bool UCharacterEquipmentComponent::IsEquipmentMenuOpened() const
{
	bool Result = false;
	if (ViewWidget.IsValid())
	{
		Result = ViewWidget->IsVisible();
	}
	return Result;
}

void UCharacterEquipmentComponent::OnWeaponChanged() const
{
	if (EquippedWeapon.IsValid())
	{
		EquippedWeapon->CheckReloadRequiredForOwner();
	}
}

void UCharacterEquipmentComponent::CreateEquipmentWidget(APlayerController* PlayerController)
{
	checkf(IsValid(ViewWidgetClass), TEXT("UCharacterEquipmentComponent::CreateEquipmentWidget view widget class is not defined"));
	
	if (!IsValid(PlayerController))
	{
		return;
	}
	ViewWidget = CreateWidget<UEquipmentViewWidget>(PlayerController, ViewWidgetClass);
	ViewWidget->InitializeWidget(this);
}

uint32 UCharacterEquipmentComponent::NextItemsArraySlotIndex(uint32 CurrentSlotIndex) const
{
	if (CurrentSlotIndex == ItemsArray.Num() - 1)
	{
		return 0;
	}
	return CurrentSlotIndex + 1;
}

uint32 UCharacterEquipmentComponent::PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex) const
{
	if (CurrentSlotIndex == 0)
	{
		return ItemsArray.Num() - 1;
	}
	return CurrentSlotIndex - 1;
}

void UCharacterEquipmentComponent::CreateLoadout()
{
	AmmunitionArray.AddZeroed(static_cast<uint32>(EAmmunitionType::AT_MAX));
	for (const TPair<EAmmunitionType, int32>& AmmoPair : StartUpAmmunitionAmount)
	{
		AmmunitionArray[static_cast<uint32>(AmmoPair.Key)] = FMath::Max(AmmoPair.Value, 0);
	}

	ItemsArray.AddZeroed(static_cast<uint32>(EEquipmentSlot::ES_MAX));
	for (const TPair<EEquipmentSlot, TSubclassOf<AEquipableItem>>& ItemPair : ItemsLoadout)
	{
		if (!IsValid(ItemPair.Value))
		{
			continue;
		}
		AddEquipmentItemInSlot(ItemPair.Value, static_cast<int32>(ItemPair.Key));
	}
}

void UCharacterEquipmentComponent::OnEquippedWeaponAmmoChanged(const int32 NewAmmo) const
{
	if (OnEquippedWeaponAmmoChangedDelegate.IsBound())
	{
		const int32 NewTotalAmmo = GetAvailableAmmunitionForEquippedWeapon();
		OnEquippedWeaponAmmoChangedDelegate.Broadcast(NewAmmo, NewTotalAmmo);
	}
}

void UCharacterEquipmentComponent::UpdateAmmo() const
{
	if (EquippedWeapon.IsValid())
	{
		OnEquippedWeaponAmmoChanged(EquippedWeapon->GetCurrentAmmo());
	}
	else
	{
		OnEquippedWeaponAmmoChanged(0);
	}
}

void UCharacterEquipmentComponent::BroadcastOnWeaponChanged() const
{
	if (OnWeaponChangedDelegate.IsBound())
	{
		OnWeaponChangedDelegate.Broadcast();
	}
}

void UCharacterEquipmentComponent::OnEquippedWeaponReloaded()
{
	ReloadAmmoInEquippedWeapon();
}

void UCharacterEquipmentComponent::ReloadAmmoInEquippedWeapon(const int32 NumberOfAmmo, const bool CheckIsFull)
{
	int32 AvailableAmmunition = GetAvailableAmmunitionForEquippedWeapon();
	const int32 CurrentAmmo = EquippedWeapon->GetCurrentAmmo();
	const int32 AmmoToReload = EquippedWeapon->GetMaxAmmo() - CurrentAmmo;
	int32 ReloadedAmmo = FMath::Min(AvailableAmmunition, AmmoToReload);
	
	if (NumberOfAmmo > 0)
	{
		ReloadedAmmo = FMath::Min(ReloadedAmmo, NumberOfAmmo);
	}
	
	AmmunitionArray[static_cast<uint32>(EquippedWeapon->GetAmmoType())] -= ReloadedAmmo;
	EquippedWeapon->SetAmmo(ReloadedAmmo + CurrentAmmo);

	if (CheckIsFull)
	{
		AvailableAmmunition = GetAvailableAmmunitionForEquippedWeapon();
		if (AvailableAmmunition == 0 || EquippedWeapon->IsFullClip())
		{
			EquippedWeapon->StopReload(true);
		}
	}
}

int32 UCharacterEquipmentComponent::GetAvailableAmmunitionForEquippedWeapon() const
{
	if (EquippedWeapon.IsValid())
	{
		return AmmunitionArray[static_cast<uint32>(GetEquippedRangeWeapon()->GetAmmoType())];
	}
	return 0;
}
