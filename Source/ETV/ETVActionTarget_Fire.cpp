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

void UETVActionTarget_Fire::Init(AETVShip* OwnerShipPtr, AETVWeapon* OwnerWeaponPtr)
{
	Super::Init(OwnerShipPtr, OwnerWeaponPtr);

	if (OwnerWeapon != nullptr)
	{
		FString NameWithWeapon = Name.ToString() + " " + OwnerWeapon->GetDisplayString();
		Name = FName(*NameWithWeapon);
	}
}

bool UETVActionTarget_Fire::CanActivate()
{
	// Check if a weapon is set
	return Super::CanActivate() && OwnerWeapon != nullptr;
}

bool UETVActionTarget_Fire::CanPerform()
{
	if (Super::CanPerform())
	{
		// Check if enemy, required type is ship (checked in Super) so casting is safe
		AETVShip* SelectedShip = Cast<AETVShip>(SelectedTarget);
		return SelectedShip->IsEnemy();
	}

	return false;
}

void UETVActionTarget_Fire::ApplyEffectsTarget()
{
	if (IsLastPerform())
	{
		Super::ApplyEffectsTarget();
		// TODO Lower health, show explosion animation
	}
}

void UETVActionTarget_Fire::ApplyEffectsSelf()
{
	if (IsLastPerform())
	{
		Super::ApplyEffectsSelf();
		// TODO Lower ammo count, recharge or similar
	}
}
