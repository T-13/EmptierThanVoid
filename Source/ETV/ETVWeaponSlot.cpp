// Copyright (C) Team13. All rights reserved.

#include "ETVWeaponSlot.h"

// Constructs default WeaponSlot
UETVWeaponSlot::UETVWeaponSlot() : Super()
{
	Weapon = nullptr;
}
// Constructs the WeaponSlot with specified parameters
UETVWeaponSlot::UETVWeaponSlot(float Size, float Weight) : Super()
{
	MaxCarryWeight = Weight;
	MaxCarrySize = Size;
	Weapon = nullptr;
}

bool UETVWeaponSlot::DoesWeaponFit(AETVWeapon * NewWeapon)
{
	return (NewWeapon->GetSize() <= MaxCarrySize && NewWeapon->GetWeight() <= MaxCarryWeight);
}

bool UETVWeaponSlot::FitWeapon(AETVWeapon * NewWeapon)
{
	if (DoesWeaponFit(NewWeapon) && !hasWeapon())
	{
		Weapon = NewWeapon;
		return true;
	}
	return false;
}

bool UETVWeaponSlot::RemoveWeapon()
{
	if (hasWeapon())
	{
		Weapon = nullptr;
		return true;
	}
	return false;
}

AETVWeapon* UETVWeaponSlot::getWeapon()
{
	return Weapon;
}

bool UETVWeaponSlot::hasWeapon()
{
	if(Weapon == nullptr)
	{
		return false;
	}
	return true;
}
