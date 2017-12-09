// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget_Fire.h"
#include "ETVShip.h"

// Sets default values
UETVActionTarget_Fire::UETVActionTarget_Fire() : Super()
{
	// Set required target type to general ship
	RequiredTargetType = AETVShip::StaticClass();
}

bool UETVActionTarget_Fire::CanPerform()
{
	return Super::CanPerform() && OwnerWeapon != nullptr && OwnerShip->IsEnemy();
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
