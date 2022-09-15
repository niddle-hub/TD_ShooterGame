#include "PlayerHUDWidget.h"

#include "Blueprint/WidgetTree.h"
#include "CharacterAttributesWidget.h"
#include "TD_ShooterGame/UI/Widgets/Weapon/WeaponAmmoWidget.h"

UWeaponAmmoWidget* UPlayerHUDWidget::GetWeaponAmmoWidget() const
{
	return WidgetTree->FindWidget<UWeaponAmmoWidget>(WeaponAmmoWidgetName);
}

UCharacterAttributesWidget* UPlayerHUDWidget::GetCharacterAttributesWidget() const
{
	return WidgetTree->FindWidget<UCharacterAttributesWidget>(CharacterAttributesWidgetName);
}
