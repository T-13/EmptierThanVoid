// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget_Use.h"
#include "ETVCalculator.h"

// Sets default values
UETVActionTarget_Use::UETVActionTarget_Use() : Super()
{
	Name = FName(TEXT("Use"));

	// Set required target type to general ship
	RequiredTargetType = AETVShip::StaticClass();
}

bool UETVActionTarget_Use::CanActivate()
{
	// Check if a weapon is set
	return Super::CanActivate() && OwnerWeapon != nullptr;
}

void UETVActionTarget_Use::ApplyEffectsTarget()
{
	Super::ApplyEffectsTarget();

	// TODO Show explosion animation

	AETVShip* SelectedShip = Cast<AETVShip>(SelectedTarget); // Required type is ship (checked in parent) so casting is safe
	UETVCalculator::CalculateWeaponEffect(OwnerShip, OwnerWeapon, SelectedShip);
}

void UETVActionTarget_Use::ApplyEffectsSelf()
{
	Super::ApplyEffectsSelf();

	// TODO Lower ammo count, recharge or similar
}
