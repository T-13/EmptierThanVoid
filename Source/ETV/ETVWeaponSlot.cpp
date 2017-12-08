// Copyright (C) Team13. All rights reserved.

#include "ETVWeaponSlot.h"

// Constructs default WeaponSlot
UETVWeaponSlot::UETVWeaponSlot() : Super()
{
	Weapon = nullptr;
}
// Constructs the WeaponSlot with specified parameters
void UETVWeaponSlot::Init(float Size, float Weight)
{
	MaxCarryWeight = Weight;
	MaxCarrySize = Size;
	//Weapon = nullptr;
}

bool UETVWeaponSlot::DoesWeaponFit(AETVWeapon * NewWeapon)
{
	return (NewWeapon->GetSize() <= MaxCarrySize && NewWeapon->GetWeight() <= MaxCarryWeight);
}

bool UETVWeaponSlot::FitWeapon(AETVWeapon * NewWeapon)
{
	if (DoesWeaponFit(NewWeapon) && !HasWeapon())
	{
		Weapon = NewWeapon;
		return true;
	}
	return false;
}

bool UETVWeaponSlot::RemoveWeapon()
{
	if (HasWeapon())
	{
		Weapon = nullptr;
		return true;
	}
	return false;
}

AETVWeapon* UETVWeaponSlot::GetWeapon()
{
	return Weapon;
}

bool UETVWeaponSlot::HasWeapon()
{
	if (Weapon == nullptr)
	{
		return false;
	}
	return true;
}
