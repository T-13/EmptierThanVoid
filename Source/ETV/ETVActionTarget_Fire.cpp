// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget_Fire.h"

// Sets default values
UETVActionTarget_Fire::UETVActionTarget_Fire() : Super()
{
	// Set required target type to general ship
	RequiredTargetType = AETVShip::StaticClass();
}

bool UETVActionTarget_Fire::CanPerform()
{
	// TODO Check if ship is selected target, check if weapon used is operable
	return Super::CanPerform();
}

void UETVActionTarget_Fire::ApplyEffectsTarget()
{
	// TODO Lower health, show explosion animation
}

void UETVActionTarget_Fire::ApplyEffectsSelf()
{
	// TODO Lower ammo count, recharge or similar
}
