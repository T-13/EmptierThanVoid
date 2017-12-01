// Copyright (C) Team13. All rights reserved.

#include "ETVWeaponTorpedo.h"
#include "Kismet/KismetMathLibrary.h"

AETVWeaponTorpedo::AETVWeaponTorpedo() : Super()
{
	Type = AETVWeapon::DamageHull;
}

AETVWeaponTorpedo::AETVWeaponTorpedo(FName NewName, float MaximumHP, float Dmg, float WeaponWeight, float WeaponSize, int32 Range, int32 Speed) : Super(NewName, MaximumHP, Dmg, WeaponWeight,  WeaponSize, Range)
{
	Type = AETVWeapon::DamageHull;

	SquaresPerTurn = Speed;
}

AETVWeaponTorpedo::AETVWeaponTorpedo(FName newName, int32 PowerLvl) : Super(newName, PowerLvl)
{
	Type = AETVWeapon::DamageHull;

	if (PowerLvl < 50)
		PowerLvl = 50;
	else if (PowerLvl > 200)
		PowerLvl = 200;

	// Balancing the speed compared to power compared to lvl
	if (Damage > PowerLvl)
		SquaresPerTurn = FMath::RandRange(1, 3);
	else
		SquaresPerTurn = FMath::RandRange(2, 5);
}

int32 AETVWeaponTorpedo::getSquaresPerTurn()
{
	return SquaresPerTurn;
}
