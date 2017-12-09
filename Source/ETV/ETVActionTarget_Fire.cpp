// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget_Fire.h"
#include "ETVShip.h"

// Sets default values
UETVActionTarget_Fire::UETVActionTarget_Fire() : Super()
{
	Name = FName(TEXT("Fire"));

	// Set required target type to general ship
	RequiredTargetType = AETVShip::StaticClass();
}

bool UETVActionTarget_Fire::CanPerform()
{
	// Check if a weapon is set
	if (Super::CanPerform() && OwnerWeapon != nullptr)
	{
		// Check if enemy, required type is ship (checked in Super) so casting is safe
		AETVShip* SelectedShip = Cast<AETVShip>(GetSelectedTarget());
		return SelectedShip->IsEnemy();
	}
	return false;
}

void UETVActionTarget_Fire::ApplyEffectsTarget()
{
	Super::ApplyEffectsTarget();
	// TODO Lower health, show explosion animation
}

void UETVActionTarget_Fire::ApplyEffectsSelf()
{
	Super::ApplyEffectsSelf();
	// TODO Lower ammo count, recharge or similar
}
