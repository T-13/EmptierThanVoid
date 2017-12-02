// Copyright (C) Team13. All rights reserved.

#include "ETVWeaponLaser.h"

AETVWeaponLaser::AETVWeaponLaser() : Super()
{
	Type = AETVWeapon::DamageShieldThenHull;
}

AETVWeaponLaser::AETVWeaponLaser(FName NewName, float MaximumHP, float Dmg, float WeaponWeight, float WeaponSize, int32 Range) : Super(NewName, MaximumHP, Dmg, WeaponWeight, WeaponSize, Range)
{
	Type = AETVWeapon::DamageShieldThenHull;
}

AETVWeaponLaser::AETVWeaponLaser(FName newName, int32 PowerLvl) : Super(newName, PowerLvl)
{
	Type = AETVWeapon::DamageShieldThenHull;
}


